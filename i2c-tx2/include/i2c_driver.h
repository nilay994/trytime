#ifndef _I2C_DRIVER_H
#define _I2C_DRIVER_H

#include <errno.h>          // error code
#include <string.h>         // error code to debug message
#include <stdio.h>          // debug prints
#include <stdint.h>         // uint8_t
#include <unistd.h>         // sleep
#include <linux/i2c.h>      // I2C datatype
#include <linux/i2c-dev.h>  // I2C datatype
#include <sys/ioctl.h>      // I2C transaction
#include <fcntl.h>          // I2C file handler

class I2C {
    public:
        I2C(uint8_t address = 0x10, uint8_t bus = 1);
        ~I2C();

        bool openI2C();
        void closeI2C();
        int readwriteI2C(uint8_t* write_bytes, uint8_t* read_bytes, uint8_t write_len, uint8_t read_len);

    private:
        int I2CFileHandle;          // File handle to open the I2C
        uint8_t I2CBus;             // I2C bus on the TX2
        uint8_t I2CAddress;         // I2C address of the sensor
};

#endif /* _I2C_DRIVER_H */
