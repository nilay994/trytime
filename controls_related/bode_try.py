from re import T
import numpy as np
import control.matlab as ml
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from numpy.core.function_base import linspace

plt.style.use('seaborn-dark')

num = np.array([1.5])
# den = np.polymul(np.array([1,1]), np.array([1,100]))
den = np.array([1, 14, 40.02])
# den = np.polymul(den, [1, 0])
G = ml.tf(num, den)
gain = ml.dcgain(G)
G = 1/gain * G

# gs = gridspec.GridSpec(1, 2) 

bode_f = plt.figure()
[mag, phase, w] = ml.bode(G, Hz=True, deg=True, margins=True)
mag_axis, phase_axis = bode_f.get_axes()
mag_axis.grid(True, which='both')
phase_axis.grid(True, which='both')

plt.figure()
t = linspace(0, 5, num=1000)
out, t = ml.step(G, T=t)
plt.plot(t, out)
plt.show()