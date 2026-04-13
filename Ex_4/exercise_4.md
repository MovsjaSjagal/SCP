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

## Part 6: Plot inspection

### 1
The sampling spectra plot, specifically the undersampled spectrum, makes aliasing the easiest to see. In this plot the sampling rate is deliberately set lower than the Nyquist rate for the signal's highest frequency components.

### 2
in the short-record spectrum plot. 

### 3
Yes, in the coupled-oscillator FFT plot, the dominant peaks from the numerical simulation line up extremely well with the theoretical reference frequencies. 

### 4
While the alignment is close, they are rarely perfectly identical due to artifacts of the discrete Fourier transform and numerical integration. A primary reason is the discrete bin spacing of the FFT; the true theoretical frequency usually falls somewhere between two calculated frequency bins, leading to a slight mismatch. Additionally, a finite observation time causes spectral leakage, and small numerical errors accumulated by the ODE solver's tolerance limits can introduce minor phase drifts.

---

## Part 7: Small code modifications

I chose option A: Change the sampling rate

When modifying the sampling rate $f_s$, we predict that aliasing will occur if any true frequency in the signal exceeds the Nyquist frequency, defined as $f_N = f_s / 2$. If the sampling rate drops below this critical threshold, the higher frequency components will fold back into the lower frequency range. The new apparent peak positions can be predicted by calculating the difference between the true frequency and the nearest multiple of the sampling rate. Upon rerunning the simulation, the resulting FFT output matches this prediction, so we see false peaks at the folded frequencies within the bounds of the FFT's discrete bin spacing.

## Part 8: 
I must say I knew quite a lot about Fourier transforms from a mathematical point of view, as I studied it in the context of generalized functions as written by Lighthill. Mathematics, however, lives in the ideal world. I now have a much clearer understanding of how a signal is sampled and then naturally how it affects the accuracy of the following Fourier process. From a program-building point of view I have learnt a lot, as I was much less familiar. I have undertand (and for the first time downloaded) make, gnuplot, etc. The directory management especially was nice to see, as mine are generally not structured as nicely. Honestly I would, as a suggestion, even prefer some deeper explanation of Makefile. I now added it to some of my other exercises to try and improve them, but actually writing a file is still quite different. AI helps of course, but AI sometimes has difficulty doing things on the specific platform and DIR that you are working with... Then again, I understand Fourier transforms are the goal. 
