#include <iostream>
#include <stdint.h>
#include <string.h>

#include "uart_struct.h"
#include "uart_driver.h"

// sudo gtkterm -b 8 -t 1 -s 115200 -p /dev/ttyTHS2

int main(int agv, char** argv) {

    SerialPort *obj = new SerialPort("/dev/ttyTHS2");
    input_dev_t input_dev;

    while(1) {

        if (obj->valid_uart_message_received) {
            // for (int i=0; i<FRAMELEN; i++) {
            //     printf("0x%02x,", obj->uart_msg_bytes[i]);
            // }
            // printf("\n");
            memcpy(&input_dev, &(obj->uart_msg_bytes[1]), FRAMELEN);
            std::cout << "val: " << input_dev.pot << std::endl;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
    return 0;
}
