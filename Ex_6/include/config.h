#ifndef CONFIG_H
#define CONFIG_H

#define CONFIG_MAX_ENTRIES 64
#define CONFIG_MAX_LEN     128

typedef struct {
    char key[CONFIG_MAX_LEN];
    char value[CONFIG_MAX_LEN];
} ConfigEntry;

typedef struct {
    ConfigEntry entries[CONFIG_MAX_ENTRIES];
    int count;
} Config;

/* Load key=value pairs from file. Exits on failure. */
void   config_load(Config* cfg, const char* path);

/* Return value string for key, or NULL if not found. */
const char* config_get(const Config* cfg, const char* key);

/* Convenience getters — exit if key missing. */
int    config_get_int(const Config* cfg, const char* key);
double config_get_double(const Config* cfg, const char* key);

#endif /* CONFIG_H */
