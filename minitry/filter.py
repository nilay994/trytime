# Interesting article at https://www.embeddedrelated.com/showarticle/779.php
# 1. IIR and FIR in digital, only IIR in analog: cap + inductors will always hold
# 2. IIR vs FIR: ability to return back to initial state after impulse
# 3. Non-steep cut-off IIR can be made by a first order TF.
# 4. easily written in code - one line, boom.
# 5. tf in time domain written as: y += alpha * (x - y); alpha = t/tau;
# 6. noise level (p-p) is suppressed by a factor of sqrt(alpha)
# 7. trade-off: vote for less noise but could lead to distortion and phase-lag


import matplotlib.pyplot as plt
import numpy as np
np.random.seed(123456789)   # repeatable results

f0 = 4
t = np.arange(0,1.0,1.0/65536)
mysignal = (np.mod(f0*t,1) < 0.5)*2.0-1
mynoise = 1.0*np.random.randn(*mysignal.shape)

plt.figure(figsize=(8,6))
plt.plot(t, mysignal+mynoise, 'gray',
         t, mysignal,         'black');

plt.show()

