CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
LDFLAGS = -lm

TARGET = legendre

.PHONY: all run plot-python clean

all: $(TARGET)

$(TARGET): src/legendre.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: $(TARGET)
	@mkdir -p output
	./$(TARGET)
	python3 scripts/reference.py

plot-python: run
	python3 scripts/plot_errors.py

clean:
	rm -f $(TARGET)
	rm -rf output
