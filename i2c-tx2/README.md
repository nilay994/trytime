## i2c-tx2 example

I2C example driver for tx2, currently running: tfmini TOF sensor for altitude.

Project still has TODOs.

connection steps:
- tfmini requires 5V, GND, SDA and SCL and sits on 0x10 (7bit addressing? not sure, else 0x20)
- test via `sudo i2cdetect -y -r 1` to scan for i2c addresses (not very effective)
- Connector J13 on Auvidia carrier J120

build steps:

```
cd build
cmake ..
make
```

execute steps:
`./i2c_example`


