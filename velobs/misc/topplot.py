import matplotlib.pyplot as plt
import numpy as np
from numpy import genfromtxt

def config_matplotlib():
    plt.rcParams['figure.figsize'] = (10, 8)
    gray = "444444"
    plt.rcParams['axes.facecolor'] = 'f5f5f5'
    plt.rcParams['axes.edgecolor'] = gray
    plt.rcParams['grid.linestyle'] = '-'
    plt.rcParams['grid.alpha'] = 0.8
    plt.rcParams['grid.color'] = 'white'
    plt.rcParams['grid.linewidth'] = 2
    plt.rcParams['axes.axisbelow'] = True
    plt.rcParams['axes.labelcolor'] = gray
    plt.rcParams['text.color'] = gray
    plt.rcParams['xtick.color'] = gray
    plt.rcParams['ytick.color'] = gray


def main():
    data = genfromtxt('pprzdata.txt', delimiter=',')
    config_matplotlib()
    plt.minorticks_on()
    plt.axis('equal')

    for i in range(np.size(data,0)):
        plt.plot(data[i,0], data[i,1], '.r')
        plt.plot(data[i,4], data[i,5], '.b')
        plt.xlim([-40, 40])
        plt.ylim([-40, 40])
        plt.grid('True')
        plt.pause(0.02)
    plt.show()

if __name__ == "__main__":
    main()