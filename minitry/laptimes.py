import matplotlib.pyplot as plt

fig, axs = plt.subplots(2, 2) #, sharey=True, tight_layout=True)
laptime_neel = [78.97, 76.84, 74.53, 75.49, 73.82, 70.54, 71.32, 73.03, 72.24, 71.19, 86.78, 125.06, 78.89, 79.63, 79.18, 85.79, 77.17, 163.47];
laptime_husain = [81.39, 75.58, 83.12, 84.99, 85.28, 130.56, 116.92, 80.39, 99.48, 129.97, 82.73, 144.41, 111.53, 248.74];
laptime_nilay = [83.99, 73.67, 69.73, 70.35, 68.58, 69.53, 69.34, 68.67, 68.78, 71.89, 73.63, 118.67, 68.48, 67.28, 66.24, 70.34, 67.03, 68.53, 156.52, 66.94];


axs[0,0].hist(laptime_neel)
axs[0,1].hist(laptime_husain)
axs[1,1].hist(laptime_nilay)

axs[0,0].set_title('Neel'); axs[0,0].grid('True');
axs[0,1].set_title('Husain'); axs[0,1].grid('True');
axs[1,1].set_title('Nilay'); axs[1,1].grid('True');

axs[0,0].set_xlabel('laptime (s)')
axs[0,1].set_xlabel('laptime (s)')
axs[1,1].set_xlabel('laptime (s)')
plt.show()
