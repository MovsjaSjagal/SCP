# Homework 4

## Part 1.
### 1. 
An FFT shows which frequencies are present in a signal and how strong they are. It converts the signal from time domain to frequency domain.

### 2. 
Sampling rate: How many samples per second are taken from the signal.
Nyquist frequency: Half the sampling rate; highest frequency that can be correctly captured.
Frequency resolution: Smallest difference between distinguishable frequencies, depends on total signal duration.

### 3.
FFT algorithms assume samples are evenly spaced in time. 

## Part 2. 
### 1.
From the lines: 
```
  wrote output/good_sampling_signal.csv
  wrote output/good_sampling_spectrum.csv
  wrote output/undersampled_signal.csv
  wrote output/undersampled_spectrum.csv  
  wrote output/short_record_signal.csv
  wrote output/short_record_spectrum.csv
  wrote output/coupled_oscillators_time.csv
  wrote output/coupled_oscillators_spectrum.csv
```
we understand that 8 files are created in a directory called output.

### 2. 
```
src/sampling_demo.c
```

### 3. 
```
src/coupled_oscillators_fft.c
```
### 4.
From the lines 
```
    peak 1: f =   50.000 Hz, amplitude = 1.0000
    peak 2: f =  120.000 Hz, amplitude = 0.7000
```
we understand that two frequencies are reported to have a peak: 50Hz and 120Hz.

## Part 3.
### 1
### 2
Nyquist frequency:
\[
f_N = \frac{f_s}{2} = \frac{128}{2} = 64 \,\text{Hz}
\]
### 3
A 120 Hz component cannot be reconstructed because it lies **above the Nyquist frequency (64 Hz)**.

Any frequency above \( f_N \) cannot be uniquely represented after sampling and will be **aliased (folded)** into a lower frequency, making the original signal unrecoverable.


### 4



### 5
#### Physical explanation
When sampling too slowly, the signal is only observed at discrete time points. A fast oscillation can appear as a slower wave because the samples “miss” the true oscillation cycles.


#### Mathematical explanation
Aliasing occurs because sampled sinusoids are indistinguishable under shifts by multiples of the sampling frequency:

$$
f_{\text{alias}} = |f - n f_s|,\quad n \in \mathbb{Z}
$$

For \( f = 120 \,\text{Hz} \), \( f_s = 128 \,\text{Hz} \):

$$
f_{\text{alias}} = |120 - 128| = 8 \,\text{Hz}
$$

Thus, a 120 Hz signal appears as an 8 Hz signal after sampling.
