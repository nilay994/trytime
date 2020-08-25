#include <i2c_driver.h>

I2C::I2C(uint8_t address, uint8_t bus) {
   I2CBus = bus;           // Default I2C bus coming out from below J120 TX2
   I2CAddress = address;   // Defaults to 0x10 for tflite
   openI2C();
}

I2C::~I2C() {
   closeI2C() ;
}

bool I2C::openI2C() {
   char fileNameBuffer[32] = {0};
   sprintf(fileNameBuffer,"/dev/i2c-%d", I2CBus);
   // TODO: recheck speed of bus - 400 kHz?
   I2CFileHandle = open(fileNameBuffer, O_RDWR);
   if (I2CFileHandle < 0) {
        // Could not open the file
        printf("[I2C] ioctl error: %s\n", strerror(errno));
        return false;
   }
   
   printf("[I2C] Device on: 0x%02x on /dev/i2c-%d\n", I2CAddress, I2CBus);

   return true;
}

// TODO: find a way to effectively close the open file descripter
void I2C::closeI2C() {
   if (I2CFileHandle > 0) {
        usleep(250);
        close(I2CFileHandle);
        usleep(250);
        //I2CFileHandle = -1;
   }
}


int I2C::readwriteI2C(uint8_t* write_bytes, uint8_t* read_bytes, uint8_t write_len, uint8_t read_len) {
   
    struct i2c_msg i2c_msgs[2];
    
    // tx msg
    i2c_msgs[0].addr = I2CAddress;
    i2c_msgs[0].flags = 0;  // write flag
    i2c_msgs[0].len = write_len; //sizeof(write_bytes);
    i2c_msgs[0].buf = &write_bytes[0];

    // rx msg
    i2c_msgs[1].addr = I2CAddress;
    i2c_msgs[1].flags = I2C_M_RD;  // read flag
    i2c_msgs[1].len = read_len; //sizeof(read_bytes);
    i2c_msgs[1].buf = &read_bytes[0];

    struct i2c_rdwr_ioctl_data i2cdata[1];
    i2cdata[0].msgs = i2c_msgs;
    i2cdata[0].nmsgs = 2;

    // Perform the transaction, device must be I2C_FUNC_I2C
    int err = ioctl(I2CFileHandle, I2C_RDWR, &i2cdata);
    if (err < 0) {
        printf("[I2C] ioctl error: %s\n", strerror(errno));
        return -1; 
    }
    // TODO: see blocking / non-blocking ioctl calls before removing delay
    usleep(10);
    return 1;
}

/*
smbus_write is deprecated? Not sure! int toReturn = i2c_smbus_write_byte(I2CFileDescriptor, writeValue);
may expect to ACK more than 1 byte not working, timing out...
error = write(I2CFileHandle, &writeValue, 1);
if (ioctl(I2CFileHandle, I2C_SLAVE, I2CAddress) < 0) {
    // Could not open the device on the bus
    error = errno;
    printf("[I2C] ioctl error: %s\n", strerror(errno));
    return false;
}
*/
