# MAVLab Loihi Projects

This is a private repository containing MAVLab-owned projects developed for embedded applications with the Kapoho Bay (KB) neuromorphic device developed at Intel Research Lab. This device is a USB stick containing 2 Loihi chips. Each of these neuromorphic chips comprises 130 000 artificial neurons and 130 000 million synapses, organized within 128 neuromorphic cores. 

https://en.wikichip.org/wiki/intel/loihi#Kapoho_Bay_.282_chip.2C_262K_neurons.29

## Setting up the Kapoho Bay

Requirements: Linux 16.04 or higher, Python 3.5.2 (no other version), latest version of pip.

First, install Miniconda:

    wget https://repo.continuum.io/miniconda/Miniconda3-latest-Linux-x86_64.sh -O miniconda.sh
    bash miniconda.sh

Create the Loihi environment running Python 3.5.2 (mandatory):

    conda create --channel conda-forge --name loihi python=3.5.2
    source activate loihi
    conda install numpy cython

Downolad the NxSDK-0.9 from Intel and install it while the conda Loihi environment is activated: 

    scp juliendupeyroux@ncl-rv210.research.intel-research.net:/nfs/ncl/releases/0.9/nxsdk-0.9.tar.gz .
    scp juliendupeyroux@ncl-rv210.research.intel-research.net:/nfs/ncl/releases/0.9/nxsdk-apps-0.9.tar.gz .
    pip install nxsdk-0.9.tar.gz

When complete, deactivate the virtual environment by running 

    conda deactivate

Plugin the KB to your machine and verify the USB device is recognized. To do so, run `lsusb` or `lsusb -t` to see if the KB is correctly recognized. Then, remove `ftdi_sio` kernel modules by running: 

    sudo rmod ftdi_sio 

Copy `99-kapoho_bay.rules` from `nxsdk-apps/utils` to `/etc/udev/rules.d` and then run:

    sudo udevadm control --reload

Install gcc and g++ `multilib`, `libgflags-dev`, `libgoogle-glog-dev` and `libprotobuf`: 

    sudo apt-get install gcc-multilib
    sudo apt-get install g++-multilib
    sudo apt-get install libgflags-dev
    sudo apt install libgoogle-glog-dev
    wget http://ftp.debian.org/debian/pool/main/p/protobuf/libprotobuf17_3.6.1-4~bpo9+1_amd64.deb
    sudo dpkg -i libprotobuf17_3.6.1-4~bpo9+1_amd64.deb

Now, test the FPIO by running the following command while the Loihi conda environment is activated: 

    conda activate loihi
    `python3 -c "import nxsdk; print(nxsdk.__path__[0])"`/bin/x86/kb/lakemont_driver --test-fpio

The output you should get is:

    Using Kapoho Bay serial number 420
    test_fpio_loopback: chips=2 num=10000 len=16 time=139248us => 1.14903Mb/s

IMPORTANT: it is necessary to reload the rules by running `sudo udevadm control --reload` and then `export KAPOHOBAY=1` after plugging in the KB, and each time you restart your computer.

Your environment is now fully set up.

## I/O UART Communication with Paparazzi via USB (FTDI driver)

### Overall picture

A schematic of the demo developed by Fede and Nilay is the following:

```
Paparazzi --(D,Ddot)--> uart_struct --(D,Ddot)--> uart_shm (Loihi)
                                                      |
Paparazzi <--(thrust)-- uart_struct <--(thrust)--------                   
```

The `loihi_lander` paparazzi branch from tudelft's repository (https://github.com/tudelft/paparazzi/tree/loihi_lander) contains the two modules that are required for this project.

- **spiking_landing (SL)**: Module developed by Jesse to perform landings of a Bebop 2 using the TinySNN library.
- **uart_driver (UART)**: Module developed by Nilay and adapted by Fede with UART protocols to send and receive information through the micro USB connection of the Bebop 2.

These two modules have been modified so SL (which is linked with the ABI optical flow messages) triggers the transmission (TX) of divergence and divergence_dot through the serial connection. Therefore, UART's TX routines run at SL frequency. The receiving (RX) of thrust values is event triggered, but this time through the incoming bytes from the serial connection.

The `uart_struct` module (https://github.com/nilay994/trytime/tree/master/uart_struct) is the C++ program developed by Nilay and Fede that reads the bytes sent by Paparazzi on the other extreme of the USB cable. Therefore, **this program is meant to be executed in the board to which the Loihi is connected ONCE PAPARAZZI HAS ALREADY STARTED**. Besides reading, it creates TX and RX shared memory addresses for the the communication with the Loihi's ROS module. Once the Loihi has computed thrust, this bridge module reads it from the shared memory and sends it back to paparazzi.

The `uart_shm` module (https://github.com/nilay994/trytime/tree/master/uart_shm) is the C++ program developed by Fede that shows the funcionalities that need to be added to the Loihi's ROS module to be able to get (and send) values from (to) paparazzi. Note that this module doesn't create but open the shared memory addresses created by `uart_struct`. **Therefore, it needs to be executed after starting uart_struct**.

### TODOs

- Remove TinySNN functionalities from the SL paparazzi module. These functions now need to be performed by the Loihi and its ROS module.
- Add the shared shared memory communication in `uart_shm` to the Loihi's ROS module and substitute `uart_shm` with it.

### Reproducibility

To reproduce the demo developed by Nilay and Fede, follow these steps:

1. Ask Nilay about the connection that you need for your application (PC/Jetson/Odroid/Raspberry).
2. Flash a Bebop 2 with the loihi_lander branch from tudelft's repo.
3. Clone Nilay's `trytime` repository:
```
git clone https://github.com/nilay994/trytime.git
git checkout working
```
4. Run paparazzi from terminal:
```
telnet 192.168.42.1
cd data/ftp/internal_000/paparazzi/
killall -9 ap.elf
./ap.elf
```
5. In a different terminal, run the "bridge" module `uart_struct`:
```
cd trytime

cd uart_struct
mkdir build
cd build
cmake ..
make
./uart_serial_example
```
6. In a different terminal, run the "fake Loihi" module `uart_shm`:
```
cd trytime

cd uart_shm
mkdir build
cd build
cmake ..
make
./uart_shm_example
```

Note that all modules have a #define DBG that you should enable in case you want to see something :)

**Final note**: If the `uart_struct` module doesn't synchronize well with paparazzi, you'll start seing a bunch of `[UART packet] Header and Footer not found` messages in the terminal, which means that the messages are not being sent correctly. This is a rare issue that can just be fixed by restarting the `uart_struct` module.
