#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Remove leading and trailing whitespace in-place. */
static void trim(char* s) {
    /* trailing */
    int len = (int)strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t' ||
        s[len - 1] == '\n' || s[len - 1] == '\r'))
        s[--len] = '\0';
    /* leading */
    int start = 0;
    while (s[start] == ' ' || s[start] == '\t') start++;
    if (start) memmove(s, s + start, len - start + 1);
}

void config_load(Config* cfg, const char* path) {
    cfg->count = 0;
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "config: cannot open '%s'\n", path);
        exit(EXIT_FAILURE);
    }

    char line[256];
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

        char* eq = strchr(line, '=');
        if (!eq) continue;

        *eq = '\0';
        char* k = line;
        char* v = eq + 1;
        trim(k);
        trim(v);

        if (cfg->count >= CONFIG_MAX_ENTRIES) {
            fprintf(stderr, "config: too many entries (max %d)\n", CONFIG_MAX_ENTRIES);
            break;
        }

        if (strlen(k) >= CONFIG_MAX_LEN) k[CONFIG_MAX_LEN - 1] = '\0';
        if (strlen(v) >= CONFIG_MAX_LEN) v[CONFIG_MAX_LEN - 1] = '\0';
        snprintf(cfg->entries[cfg->count].key, CONFIG_MAX_LEN, "%s", k);
        snprintf(cfg->entries[cfg->count].value, CONFIG_MAX_LEN, "%s", v);
        cfg->count++;
    }
    fclose(f);
}

const char* config_get(const Config* cfg, const char* key) {
    for (int i = 0; i < cfg->count; i++)
        if (strcmp(cfg->entries[i].key, key) == 0)
            return cfg->entries[i].value;
    return NULL;
}

int config_get_int(const Config* cfg, const char* key) {
    const char* v = config_get(cfg, key);
    if (!v) { fprintf(stderr, "config: missing key '%s'\n", key); exit(EXIT_FAILURE); }
    return atoi(v);
}

double config_get_double(const Config* cfg, const char* key) {
    const char* v = config_get(cfg, key);
    if (!v) { fprintf(stderr, "config: missing key '%s'\n", key); exit(EXIT_FAILURE); }
    return atof(v);
}
