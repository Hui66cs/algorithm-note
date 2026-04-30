# FFT Application Report 

## Introduction

The classical algorithmic fact is well known: the FFT evaluates the DFT and its inverse in $\Theta(N \log N)$ time rather than $\Theta(N^2)$. However, an important distinction must be made between mathematical theory and computer science. While the beautiful properties of diagonalizing convolution and operators belong to the continuous Fourier Transform, it is strictly the $\Theta(N \log N)$ algorithmic breakthrough of the FFT that brings these properties out of theoretical math and makes them feasible for real-time applications. 

Without the FFT's acceleration, these mathematical properties would remain computational impossibilities for large-scale systems. In this report, I explore 5 major applications. I intentionally include variants of the ordinary FFT—especially the discrete sine transform (DST), the nonuniform FFT (NUFFT), and DFT-spread OFDM—to reveal how this $\Theta(N \log N)$ engine is adapted to solve specific hardware or physical constraints [1].

## 1. OFDM and DFT-S-OFDM (Wireless Communication)

The motivation for OFDM is frequency-selective multipath. A single wideband carrier sees a long channel impulse response, forcing complicated time-domain equalizers. Modern physical-layer overviews explicitly contrast this with OFDM, which splits bandwidth into many narrow subcarriers, adds a cyclic prefix, and corrects distortion on a subcarrier-by-subcarrier basis [2]. 

A first technical fact is carrier orthogonality. Let the $k$-th baseband subcarrier on one OFDM symbol be
$$ \phi_k[n] = e^{2\pi i kn/N}, \qquad 0 \le n < N. $$
For $k \neq \ell$,
$$ \sum_{n=0}^{N-1} \phi_k[n]\overline{\phi_\ell[n]} = \sum_{n=0}^{N-1} e^{2\pi i (k-\ell)n/N} = \frac{1-e^{2\pi i (k-\ell)}}{1-e^{2\pi i (k-\ell)/N}} = 0. $$
Therefore, the $N$ subcarriers are mutually orthogonal on the FFT interval, allowing the receiver to separate them via DFT.

The second fact is what makes OFDM powerful over dispersive channels. Let $s \in \mathbb{C}^N$ be the data symbols and let the transmitter send $x = F_N^{*} s$, where $F_N$ is the unitary DFT matrix. Suppose the channel impulse response is $h[0],\dots,h[L-1]$, and the cyclic prefix length $G \ge L-1$. After removing the prefix, the retained samples satisfy:
$$ y[n] = \sum_{\ell=0}^{L-1} h[\ell]\,x[(n-\ell)\bmod N] + w[n]. $$
The effective channel matrix is not Toeplitz but circulant: $y = C(h)x + w$. This is the algebraic payoff of the cyclic prefix.

Because circulant matrices are diagonalized by the Fourier matrix, there exists a diagonal matrix $\Lambda_H = \operatorname{diag}(H_0,\dots,H_{N-1})$ such that $F_N\, C(h)\, F_N^{*} = \Lambda_H$. Hence:
$$ Y = F_N y = \Lambda_H s + W, \implies Y_k = H_k s_k + W_k. $$
The DFT columns are eigenvectors of every circulant matrix. Therefore, equalization is simply $\hat{s}_k = Y_k/H_k$. Since the FFT computes this in $\Theta(N\log N)$ instead of $\Theta(N^2)$, the battery-powered receiver can demodulate millions of symbols per second in real-time. 

A related variant is **DFT-s-OFDM** (used in 5G uplink). It inserts an $M$-point DFT before subcarrier mapping to keep the waveform "essentially single-carrier," lowering the Peak-to-Average Power Ratio (PAPR) to save smartphone battery while retaining FFT-based frequency-domain equalization.

## 2. Spectral Differentiation and Fast Poisson Solvers (Numerical PDEs)

In scientific computing, the FFT becomes part of the numerical method itself. In Fourier spectral methods, one approximates a periodic function by a trigonometric polynomial and uses the FFT to shuttle between grid values and coefficients efficiently [3].

Let $u_N(x)=\sum_{|k|\le K}\hat{u}_k e^{ikx}$ be a trigonometric interpolant. Differentiation acts coefficient-wise:
$$ u_N^{(m)}(x)=\sum_{|k|\le K}(ik)^m \hat{u}_k e^{ikx}. $$
The algorithm is: FFT to obtain $\hat{u}_k$, multiply by $(ik)^m$, and IFFT to return to physical space. The FFT converts calculus into simple arithmetic.

The Poisson equation ($-\Delta u = f$) shows why this matters. With periodic boundaries and mean-zero $f$, we write $f$ and $u$ as 2D Fourier series. Since $\partial_{xx} e^{i(kx+\ell y)} = -k^2 e^{i(kx+\ell y)}$, matching coefficients gives:
$$ (k^2+\ell^2)\hat u_{k\ell} = \hat f_{k\ell} \implies \hat u_{k\ell}=\frac{\hat f_{k\ell}}{k^2+\ell^2}. $$
A dense spatial approach would couple each unknown to many others, but Fourier diagonalization reduces the PDE to independent scalar divisions. The FFT enables solving this on an $N\times N$ grid in $\Theta(N^2 \log N)$ time. 

For fixed boundary values (zero-Dirichlet), the discrete Laplacian is diagonalized by sine eigenvectors. The **Discrete Sine Transform (DST)**—a variant of the FFT—reduces the Poisson solve to roughly $2N^2 \log_2 N$ operations, proving that the FFT architecture adapts to varying physical constraints.

## 3. Medical Imaging (MRI and NUFFT)

In MRI, the Fourier interpretation is built into the physics. The measured signal and the image are Fourier-transform pairs. Rectilinear k-space sampling leads to image pixels via a 2D Inverse FFT. Thus, the "raw data" of MRI are actually spatial frequencies.

This leads to the **Nonuniform FFT (NUFFT)**. Standard FFT requires uniformly spaced frequencies [4]. However, modern rapid MRI uses spiral or radial trajectories. The NUFFT variant interpolates irregular samples onto an oversampled Cartesian lattice so the standard FFT can still execute the heavy computation, making fast, high-resolution MRI practical.

## 4. Radar Range-Doppler Processing

Modern FMCW (Frequency Modulated Continuous Wave) radar relies on a dual-FFT architecture. A "range FFT" is applied to the samples of each chirp, converting delay into distance. Then, a "Doppler FFT" is applied across consecutive chirps, converting phase progression into velocity. The $\Theta(N \log N)$ efficiency of the FFT ensures that an autonomous vehicle can process these 2D FFTs continuously to identify targets in real-time.

## 5. Audio, Speech, and Music (STFT)

Since standard FFT loses temporal information, the **Short-Time Fourier Transform (STFT)** was developed [5]. By windowing a signal and applying repeated FFTs to short consecutive frames, STFT produces a spectrogram (time vs. frequency). Without the algorithmic speed of the FFT, processing thousands of overlapping windows per second would be impossible. This FFT variant serves as the backbone for modern digital phase vocoders, pitch correction (Auto-Tune), and noise cancellation [6].

## Conclusion

Across all five applications, the same principle reappears: the FFT is valuable when the Fourier basis exposes hidden simplicity. In OFDM, it diagonalizes cyclic convolution; in spectral PDE solvers, it transforms calculus into division; in MRI, it inverts the physical measurement model; in radar, it extracts speed from phase; and in audio, it enables real-time spectral mapping. 

Ultimately, this exploration reveals a profound distinction between math and computer science. While the beautiful properties of diagonalizing convolution belong to the continuous Fourier Transform, it is strictly the $\Theta(N \log N)$ algorithmic breakthrough of the FFT that bridged the gap between theory and reality. Without the FFT's acceleration, these mathematical properties would remain computational impossibilities. In this sense, the applications discussed here are not merely applications of a math formula, but testaments to how a single leap in algorithmic efficiency can birth entire industries.

## References

[1] M. Frigo and S. G. Johnson, "The Design and Implementation of FFTW3," *Proceedings of the IEEE*, vol. 93, no. 2, pp. 216-231, 2005.
[2] D. Tse and P. Viswanath, *Fundamentals of Wireless Communication*. Cambridge University Press, 2005.
[3] L. N. Trefethen, *Spectral Methods in MATLAB*. SIAM, 2000.
[4] J. A. Fessler and B. P. Sutton, "Nonuniform fast Fourier transforms using min-max interpolation," *IEEE Transactions on Signal Processing*, vol. 51, no. 2, pp. 560-574, 2003.
[5] J. B. Allen, "Short term spectral analysis, synthesis, and modification by discrete Fourier transform," *IEEE Transactions on Acoustics, Speech, and Signal Processing*, vol. 25, no. 3, pp. 235-238, 1977.
[6] M. R. Portnoff, "Implementation of the digital phase vocoder using the fast Fourier transform," *IEEE Transactions on Acoustics, Speech, and Signal Processing*, vol. 24, no. 3, pp. 243-248, 1976.