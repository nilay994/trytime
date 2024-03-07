## Plot results from mutex_try, check consistency of timestamps

import matplotlib.pyplot as plt
import numpy as np

import seaborn as sns
sns.set()

data = np.genfromtxt("thread_try.csv", delimiter=',', skip_footer=1)

plt.figure()
plt.plot(data, drawstyle='steps', label="timestamps")
plt.grid('True')
plt.legend()
plt.show()
