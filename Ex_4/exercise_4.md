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
$$
f_N = \frac{f_s}{2} = \frac{128}{2} = 64 \,\text{Hz}
$$

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

For $f = 120 \,\text{Hz}$, $f_s = 128 \,\text{Hz}$:

$$
f_{\text{alias}} = |120 - 128| = 8 \,\text{Hz}
$$

Thus, a 120 Hz signal appears as an 8 Hz signal after sampling.

## Part 4. Frequency resolution (short_record case)

### 1




### 2
FFT frequency spacing:
$$
\Delta f = \frac{f_s}{N} = \frac{1}{T}
$$

So:
$$
\Delta f = \frac{512}{64} = 8 \,\text{Hz}
$$



### 3
The two frequencies are 50 Hz and 55 Hz, but the FFT bins are spaced 8 Hz apart. This means each FFT bin represents a fairly wide frequency interval (8 Hz), but the separation between 50 Hz and 55 Hz is only 5 Hz, which is smaller than the bin spacing.

As a result both frequencies fall into the same or neighboring bin, so the FFT cannot clearly distinguish them. Hence they appear as a single broadened peak instead of two distinct peaks.


### 4
To improve separation, the first thing you should change is the (acquisition) time, since frequency resolution is essentially determined by:
$$
\Delta f = \frac{1}{T}
$$

So increasing \( T \) (longer measurement time) directly reduces \( \Delta f \), giving finer frequency bins. Modifying the output files or plotting mech obviously will not change solve this problem, since it is an actual bin spacing problem at the coding level.


## Part 5: Coupled oscillators

### 1
The equations of motion used in the code are

$$
m \ddot{x}_1 = -k x_1 - k_c (x_1 - x_2)
$$

$$
m \ddot{x}_2 = -k x_2 - k_c (x_2 - x_1)
$$

where \(k\) is the spring constant of the outer walls and \(k_c\) is the coupling spring between the two masses.



### 2
This system represents two identical masses attached to fixed walls with an additional spring coupling them. It is a standard model for coupled oscillations, and it appears in many physical contexts such as simplified molecular vibrations or small lattice models where energy can be exchanged between neighboring degrees of freedom.



### 3
The in-phase normal mode corresponds to both masses moving together, meaning \(x_1 = x_2\). In this case the coupling spring is not stretched, so the motion is governed mainly by the wall springs and the frequency becomes

$$
\omega_1 = \sqrt{\frac{k}{m}}
$$

The out-of-phase mode corresponds to the masses moving in opposite directions, \(x_1 = -x_2\), which stretches the coupling spring maximally and increases the restoring force. The frequency is therefore higher and given by

$$
\omega_2 = \sqrt{\frac{k + 2k_c}{m}}
$$



### 4
The FFT of \(x_1(t)\) contains more than one frequency because the motion of a single mass is generally not a pure normal mode. Depending on the initial conditions, both normal modes are excited at the same time, and the observed motion becomes a superposition of these oscillations. As a result, the frequency spectrum reflects multiple contributions rather than a single sharp peak.



### 5
Comparing the FFT peaks with the theoretical normal-mode frequencies is useful because it provides a direct check that the numerical simulation matches the expected physics. It confirms that the system decomposes correctly into its eigenmodes, and it also helps distinguish true physical features from numerical artifacts introduced by discretisation or finite time sampling. In practice it is one of the simplest ways to validate that the computed dynamics are consistent with the underlying analytical model.
