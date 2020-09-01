#!/usr/bin/env python
import threading
import collections
import serial
import time
import struct
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import psutil

def parseThread(ser):
    print('Connected to ' + str(ser.name))
    ser.reset_input_buffer()
    ser.reset_output_buffer()

    global dataf 
    global datal

    while (ser.is_open):
        inbyte = ser.read(1)
        if (inbyte == b'$'):
            serdata = ser.read(5)
            dataf, datal, = struct.unpack('fb', serdata)
            # print(dataf, datal)
        else:
            dataf = 0
            datal = 0
            # todo: find a way to reset and sync uart line mid reset
            ser.reset_input_buffer()

data_curr_axes_arr = []
mindatacurr = [0, 0, 0, 0]
maxdatacurr = [10, 10, 10, 10]
def serialplot(self, lines, pltNumber, axs):

    # if pltNumber == 0:  # in order to make all the clocks show the same reading
    #     currentTimer = time.perf_counter()
    #     self.plotTimer = int((currentTimer - self.previousTimer) * 1000)     # the first reading will be erroneous
    #     self.previousTimer = currentTimer
    global dataf
    global datal

    global mindatacurr, maxdatacurr

    if (pltNumber == 0):
        data_curr = dataf
    elif (pltNumber == 1):
        data_curr = datal
    elif (pltNumber == 2):
        data_curr = 0
    elif (pltNumber == 3):
        data_curr = psutil.cpu_percent()
    else:
        data_curr = 0
    
    if (data_curr > maxdatacurr[pltNumber]):
        maxdatacurr[pltNumber] = data_curr
        yLim = (mindatacurr[pltNumber]-20, maxdatacurr[pltNumber]+20)
        axs.set_ylim(yLim)
    if (data_curr < mindatacurr[pltNumber]):
        mindatacurr[pltNumber] = data_curr
        yLim = (mindatacurr[pltNumber]-20, maxdatacurr[pltNumber]+20)
        axs.set_ylim(yLim)
    
    data_curr_axes_arr[pltNumber].append(data_curr)    # we get the latest data point and append it to our array
    lines.set_data(range(100), data_curr_axes_arr[pltNumber])

def config_matplotlib():
    plt.style.use('dark_background')
    plt.rcParams['figure.figsize'] = (10, 8)
    # gray = "444444"
    # plt.rcParams['axes.facecolor'] = 'f5f5f5'
    # plt.rcParams['axes.edgecolor'] = gray
    # plt.rcParams['grid.linestyle'] = '-'
    plt.rcParams['grid.alpha'] = 0.5
    plt.rcParams['grid.color'] = '444444'
    plt.rcParams['grid.linewidth'] = 0.5
    plt.rcParams['axes.axisbelow'] = True

def main():
    config_matplotlib()
    portName = '/dev/ttyUSB0'
    baudRate = 115200
    ser = serial.Serial(portName, baudRate, timeout=4)
    print('Connected to ' + str(portName) + ' at ' + str(baudRate) + ' BAUD.')

    parse_thread = threading.Thread(target=parseThread, args=(ser,))
    parse_thread.start()


    # plotting starts below
    pltInterval = 20    # Period at which the plot animation updates [ms]
    title = ['X Acceleration', 'Y Acceleration', 'Z Acceleration', 'CPU']

    xLimit = [(0, 100), (0, 100), (0, 100), (0, 100)]
    yLimit = [(0, 4096), (0, 100), (0, 4096), (0, 100)]

    anim = []
    line = []

    fig, axs = plt.subplots(2, 2)
    axs = axs.ravel()
    plt.subplots_adjust(wspace=0.1, hspace=0.3)

    for j in range(4):   # give an array for each type of data and store them in a list
        data_curr_axes_arr.append(collections.deque([0] * 100, maxlen=100))
    
    for i in range(4):
        axs[i] = plt.subplot(2, 2, i+1)
        axs[i].set_xlim(xLimit[i])
        axs[i].set_ylim(yLimit[i])
        axs[i].set_title(title[i])
        line = axs[i].plot([], [])[0]
        axs[i].grid(True)
        # TODO: blit true doesn't help, not sure FunctionAnimation takes 100% cpu
        anim.append(animation.FuncAnimation(fig, serialplot, fargs=[line, i, axs[i]], interval=pltInterval))  # fargs has to be a tuple
    plt.show()

    
if __name__ == '__main__':
    main()
