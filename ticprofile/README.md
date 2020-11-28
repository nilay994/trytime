Profile tics of embedded processor against ROS

Helps with datapacket delivery ensure, latency and multiple rw..
Helps with decision: threading and queues vs deep copy


## betaflight diff
```
# version
# Betaflight / KAKUTEF7 (KTF7) 4.3.0 Oct 14 2020 / 13:52:12 (efa39dc0f) MSP API: 1.44

# start the command batch
batch start

board_name KAKUTEF7

# feature
feature -RX_SERIAL
feature -OSD
feature RX_MSP

# serial
serial 0 1 115200 57600 0 115200

# map
map TAER1234

# aux
aux 0 0 0 1700 2100 0 0
aux 1 1 0 900 2100 0 0
aux 2 27 2 1700 2100 0 0
aux 3 26 0 900 2100 0 0
aux 4 50 1 1700 2100 0 0

# rxfail
rxfail 0 s 1500
rxfail 1 s 1500
rxfail 2 s 1500
rxfail 3 s 875
rxfail 6 h

# master
set acc_calibration = -68,-92,-53,1
set mag_hardware = NONE
set msp_override_channels_mask = 31
set blackbox_sample_rate = 1/1
set blackbox_mode = ALWAYS
set motor_pwm_protocol = DSHOT300
set small_angle = 180
set debug_mode = RC_INTERPOLATION

profile 0

rateprofile 1

# end the command batch
batch end

```

![result](https://github.com/nilay994/trytime/raw/master/ticprofile/betaflight-msp.png "result")
![setup](https://github.com/nilay994/trytime/raw/master/ticprofile/setup.png "setup")
