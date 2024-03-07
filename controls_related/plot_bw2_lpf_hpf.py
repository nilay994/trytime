import numpy as np
import matplotlib.pyplot as plt

plt.style.use('seaborn-dark')

filt = np.loadtxt("filt.csv", delimiter=",")
est = np.loadtxt("est.csv", delimiter=",")

plt.figure()
plt.subplot(2,1,1)
plt.plot(filt[:, 0], filt[:, 1], label="measured")
plt.plot(filt[:, 0], filt[:, 2], label="filtered")
plt.xlabel('time (s)')
plt.ylabel('voltage (V)')
plt.legend()
plt.grid(True)
plt.subplot(2,1,2)
plt.plot(est[:, 0], est[:, 1], label="v_rms")
plt.xlabel('time (s)')
plt.ylabel('voltage (V)')
plt.legend()
plt.grid(True)
plt.show()
