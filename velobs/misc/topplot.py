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
    data = genfromtxt('drone_data.csv', delimiter=',')
    config_matplotlib()
    plt.minorticks_on()
    # plt.axis('equal')

    for i in range(np.size(data,0)):
        plt.plot(data[i,0], data[i,1], '.r')
        plt.plot(data[i,4], data[i,5], '.b')

        # for MAG
        # plt.plot(data[i, 8], np.rad2deg(data[i, 3]), '-r')

        print(np.round([data[i, 0], data[i, 1], data[i, 2], data[i, 3]], 3), end =" ")
        print("|", end =" ")
        print(np.round([data[i, 4], data[i, 5], data[i, 6], data[i, 7]], 3))
        plt.xlim([-100, 100])
        plt.ylim([-100, 100])
        plt.grid('True')
        plt.pause(0.05)
    plt.show()

if __name__ == "__main__":
    main()