#include <i2c_driver.h>
#include <thread>   // threading main loop
#include <chrono>   // delay for thread

#define I2C_ADDR 0x10
#define I2C_BUS 1

int main() {
    // Open I2CDevice 0x10 on I2CBus 1
    // errors just say that no other file handle is using the i2c device
    I2C *tflite = new I2C(I2C_ADDR, I2C_BUS);

    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        uint8_t write_bytes[3] = {0x01, 0x02, 0x07};
        uint8_t read_bytes[7] = {0};

        // perform transaction
        if (tflite->readwriteI2C(write_bytes, read_bytes, 3, 7) > 0) {
            // TFLITE frame: trig 0x00 dist_l dist_h pow_l pow_h mode
            uint16_t distance_i = (uint16_t) ((read_bytes[3] << 8) | read_bytes[2]);
            float distance = (float) distance_i;

            // the raw distance could need a median filter: 
            // https://github.com/tudelft/paparazzi/commit/797755d0d0f5d958ae6193b98987c8e129cbe332
            printf("distance: %f\n", distance);
        }
    }

    // disconnect i2c device
    printf("Closing I2C demo! \n");
    tflite->closeI2C();
    return 0;
}
