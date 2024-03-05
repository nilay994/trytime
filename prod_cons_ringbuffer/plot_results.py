import numpy as np
import matplotlib.pyplot as plt

producer = np.loadtxt("build//producer.csv", delimiter="\n", dtype=int)
consumer = np.loadtxt("build//consumer.csv", delimiter="\n", dtype=int)

plt.figure()
plt.plot(producer, '.', label="producer")
plt.plot(consumer, '.', label="consumer")
plt.legend()
plt.show()
