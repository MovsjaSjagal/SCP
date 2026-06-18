set terminal pngcairo size 900,600 enhanced font 'Arial,12'
set datafile separator ","
set grid
set key top right

# ── Exercise 1: LLN ───────────────────────────────────────────────────────────
set output "output/ex1_coin_toss.png"
set title "Exercise 1: Law of Large Numbers — Coin Toss"
set xlabel "Number of tosses (n)"
set ylabel "Running fraction of heads"
set logscale x
set yrange [0.3:0.7]
plot "output/coin_toss.csv" using 1:2 with lines lc rgb "steelblue" lw 1.5 title "Running fraction", \
     0.5 with lines lc rgb "red" lw 2 dt 2 title "Expected 0.5"
unset logscale x
unset yrange

# ── Exercise 2: Monte Carlo π ─────────────────────────────────────────────────
set output "output/ex2_monte_carlo_pi.png"
set title "Exercise 2: Monte Carlo Estimate of {/Symbol p} — Error vs N"
set xlabel "N (samples)"
set ylabel "|{/Symbol p}_{est} - {/Symbol p}|"
set logscale xy
plot "output/monte_carlo_pi.csv" using 1:3 with linespoints \
         lc rgb "steelblue" lw 2 pt 7 ps 1.4 title "MC error", \
     2.0/sqrt(x) with lines lc rgb "red" lw 2 dt 2 title "~1/sqrt(N) reference"
unset logscale xy

# ── Exercise 3: Change of variables ──────────────────────────────────────────
set output "output/ex3_change_of_variables.png"
set title "Exercise 3: Transformation Y = U^2"
set xlabel "y"
set ylabel "Density"
set xrange [0:1]
set yrange [0:12]
plot "output/change_of_variables.csv" using 1:2 with boxes \
         lc rgb "steelblue" fs solid 0.5 border -1 title "Empirical histogram", \
     "output/change_of_variables.csv" using 1:3 with lines \
         lc rgb "red" lw 2.5 title "Analytic f_Y(y) = 1/(2{/Symbol \326}y)"
unset xrange
unset yrange

# ── Exercise 4: Inverse transform ────────────────────────────────────────────
set output "output/ex4_exponential_pdf.png"
set title "Exercise 4: Inverse Transform — Exponential({/Symbol l} = 1.5)"
set xlabel "y"
set ylabel "Density"
plot "output/exponential_pdf.csv" using 1:2 with boxes \
         lc rgb "steelblue" fs solid 0.5 border -1 title "Empirical histogram", \
     "output/exponential_pdf.csv" using 1:3 with lines \
         lc rgb "red" lw 2.5 title "PDF: {/Symbol l} exp(-{/Symbol l}y)"

# ── Exercise 5: Empirical CDF ─────────────────────────────────────────────────
set output "output/ex5_empirical_cdf.png"
set title "Exercise 5: Empirical vs Exact CDF — Exponential({/Symbol l} = 1.5)"
set xlabel "y"
set ylabel "F(y)"
set yrange [0:1]
plot "output/empirical_cdf.csv" using 1:2 with steps \
         lc rgb "steelblue" lw 2 title "Empirical CDF", \
     "output/empirical_cdf.csv" using 1:3 with lines \
         lc rgb "red" lw 2.5 title "Exact: 1 - exp(-{/Symbol l}y)"
unset yrange
