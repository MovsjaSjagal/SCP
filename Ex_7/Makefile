CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2 -Iinclude
LDFLAGS = -lm

BINS = ex1_coin_toss ex2_monte_carlo_pi ex3_change_of_variables \
       ex4_inverse_transform ex5_empirical_cdf

.PHONY: all run plot clean

all: $(BINS)

%: src/%.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

run: all
	@mkdir -p output
	./ex1_coin_toss
	./ex2_monte_carlo_pi
	./ex3_change_of_variables
	./ex4_inverse_transform
	./ex5_empirical_cdf

plot: run
	gnuplot plots/plot_all.gp
	@echo "Plots written to output/"

clean:
	rm -f $(BINS)
	rm -rf output
