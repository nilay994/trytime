#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <deque>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <thread>
#include <atomic>
#include "uart_struct.h"

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
using namespace boost::interprocess;

#define LOIHI_RX_MEMNAME "loihi_rx"
#define LOIHI_TX_MEMNAME "loihi_tx"


int main(int argc, char** argv) {
    loihi_rx_shm *loihi_rx_shm_data;
    loihi_tx_shm *loihi_tx_shm_data;

    // RX shared memory (divergence, divergence_dot, flag)
    boost::interprocess::shared_memory_object shm_rx(open_only, LOIHI_RX_MEMNAME, read_write);
    mapped_region region_rx(shm_rx, read_write);
    void *addr_rx = region_rx.get_address();
    loihi_rx_shm_data = static_cast<loihi_rx_shm*>(addr_rx);
    std::cout << "[SHM] Linked to Loihi RX shared memory" << std::endl;

    // TX shared memory (thrust, flag)
    boost::interprocess::shared_memory_object shm_tx(open_only, LOIHI_TX_MEMNAME, read_write);
    mapped_region region_tx(shm_tx, read_write);
    void *addr_tx = region_tx.get_address();
    loihi_tx_shm_data = static_cast<loihi_tx_shm*>(addr_tx);
    std::cout << "[SHM] Linked to Loihi TX shared memory" << std::endl;

    while(1) {

        // RX
        // TODO: deep copy this 
        if (loihi_rx_shm_data->flag) {
        printf("[RX] cnt: %i, div: %f, divdot: %f\n", loihi_rx_shm_data->cnt, loihi_rx_shm_data->divergence, loihi_rx_shm_data->divergence_dot);
        loihi_rx_shm_data->flag = false;
        } else {
            continue;
        }

        // TODO: The Loihi network should be here

        // TX
        if (!loihi_tx_shm_data->flag) {
            loihi_tx_shm_data->cnt = loihi_rx_shm_data->cnt;
            loihi_tx_shm_data->thurst = loihi_rx_shm_data->divergence * 2;
            printf("[TX] cnt: %i, thrust: %f\n", loihi_tx_shm_data->cnt, loihi_tx_shm_data->thurst);
            loihi_tx_shm_data->flag = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}

