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

// #define DBG

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

    int cnt;
    float divergence, divergence_dot, thrust;
    while(1) {

        // RX
        if (loihi_rx_shm_data->flag) {
            cnt = loihi_rx_shm_data->cnt;
            divergence = loihi_rx_shm_data->divergence;
            divergence_dot = loihi_rx_shm_data->divergence_dot;
            loihi_rx_shm_data->flag = false;
        } else {
            continue;
        }
        #ifdef DBG
        printf("[RX] cnt: %i, div: %f, divdot: %f\n", cnt, divergence, divergence_dot);
        #endif

        // TODO: The Loihi network should be here
        // Get thrust from Loihi
        thrust = divergence * 2;

        // TX
        if (!loihi_tx_shm_data->flag) {
            loihi_tx_shm_data->cnt = cnt;
            loihi_tx_shm_data->thrust = thrust;
            loihi_tx_shm_data->flag = true;
        }
        #ifdef DBG
        printf("[TX] cnt: %i, thrust: %f\n", cnt, thrust);
        #endif

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}

