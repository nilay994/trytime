import matplotlib.pyplot as plt

fig, axs = plt.subplots(2, 2) #, sharey=True, tight_layout=True)

laptime_nilay = [35.05,   32.23,  31.58,  32.02,  31.32,  32.22,  30.12,  30.93,  31.53,  31.82,  30.72,  31.54,  30.27,  33.37,  28.55,  30.53,  32.13];
laptime_marijn = [30.5,28.72,27.78,28.28,27.28,26.93,26.95,27.31,27.28,27.86,26.95,27.16,26.67,27.23,27.12,27.98,27.34,27,26.95,26.77];
laptime_annemijn = [30.51,28.38,28.39,27.73,27.7,27.2,27.36,27.59,27.54,27.44,27.98,27.84,27.55,27.45,27.36,27.42,27.15,27.56,27.18,29.11];

axs[0,0].hist(laptime_nilay)
axs[0,1].hist(laptime_marijn)
axs[1,1].hist(laptime_annemijn)

axs[0,0].set_title('Nilay'); axs[0,0].grid('True');
axs[0,1].set_title('Marijn'); axs[0,1].grid('True');
axs[1,1].set_title('Annemijn'); axs[1,1].grid('True');

axs[0,0].set_xlabel('laptime (s)')
axs[0,1].set_xlabel('laptime (s)')
axs[1,1].set_xlabel('laptime (s)')
plt.show()
