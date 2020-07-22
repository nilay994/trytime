## Threaded UART driver for nvidia-jetson-tx2
Adopted from: https://github.com/uzh-rpg/rpg_quadrotor_control

- Port in use: UART1 `/dev/ttyTHS2` 
- UART1 is located on J17 of the tx2-dev-board, only tested after removing dev-board camera.
- Rights given to current user to access port without sudo access: `sudo usermod -a -G tty $USER`. A`sudo reboot now` after giving rights.
- Tested with 3.3V logic levels @ baud of 115200.
- Loopback tested with `sudo apt-get install cutecom`

Build instructions:
- `mkdir build` in this directory.
- `cd build; cmake ..` for generating a makefile.
-  `make` and run `./uart_serial_example` in the build directory, no chmod required on generated bin.

### TODO:
try April:

1) custom baud rate via constructor.
2) send callback thread via constructor?!
3) read aliasing, profile latency and dropouts.
4) see how tx works and if you need to mutex it.

try June:

5) diff between deque (byte_buf) and a normal array in the driver.
6) perfect packets in `uart_driver` thread, can't SPSC them correctly to the uart_main thread.
7) packet length is buggy in the driver (with header and footer).
