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

