from re import T
import numpy as np
import control.matlab as ml
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
from numpy.core.function_base import linspace

plt.style.use('seaborn-dark')

fs = 20 # sampling freq: 20 hz
fc = 1  # cutoff: 5 hz

def gen_2hz_sine_wave(t):
    sine_wave = 10 * np.sin(2 * np.pi * 1 * t)
    sine_wave += 2 * np.random.randn()
    return sine_wave

class FirstOrderFilter:
    def __init__(self, fc, fs):
        self.fc = fc
        self.fs = fs
        self.alpha = 1 - np.exp(-2*np.pi*fc/fs)
        self.y = 0
        print(f"Alpha: {self.alpha:.2f}, cutoff: {fc} hz, sampling: {fs} hz")

    def update(self, sample):
        self.y = self.alpha * sample + (1 - self.alpha) * self.y
        return self.y

    def reset(self):
        self.y = 0

time = np.linspace(0, 10, 10 * fs)

# initialize signals
y = np.zeros(len(time))
y_filtered = np.zeros(len(time))
filter = FirstOrderFilter(fc, fs)

# generate signal and filter
for i in range(len(time)):
    y[i] = gen_2hz_sine_wave(time[i])
    y_filtered[i] = filter.update(y[i])

# plot
plt.figure()
plt.plot(time, y, "o-", label='Noisy Signal')
plt.plot(time, y_filtered, "o-", label='Filtered Signal')  
plt.legend()
plt.grid(True)
plt.show()

# todo: add bode plot for the above filter
# num = np.array([1.5])
# # den = np.polymul(np.array([1,1]), np.array([1,100]))
# den = np.array([1, 14, 40.02])
# # den = np.polymul(den, [1, 0])
# G = ml.tf(num, den)
# gain = ml.dcgain(G)
# G = 1/gain * G

# gs = gridspec.GridSpec(1, 2) 

# bode_f = plt.figure()
# [mag, phase, w] = ml.bode(G, Hz=True, deg=True, margins=True)
# mag_axis, phase_axis = bode_f.get_axes()
# mag_axis.grid(True, which='both')
# phase_axis.grid(True, which='both')

# plt.figure()
# t = linspace(0, 5, num=1000)
# out, t = ml.step(G, T=t)
# plt.plot(t, out)
# plt.grid(True)
# plt.show()