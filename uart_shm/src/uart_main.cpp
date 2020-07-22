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
#include <boost/interprocess/sync/scoped_lock.hpp>
using namespace boost::interprocess;


#define LOIHI_RX_MEMNAME "loihi_rx"
#define LOIHI_TX_MEMNAME "loihi_tx"
loihi_rx_shm *loihi_rx_shm_data;
loihi_tx_shm *loihi_tx_shm_data;

bool createRXSharedMemory();
bool createRXSharedMemory() {
    try {

        // Create a shared memory object.
        boost::interprocess::shared_memory_object shm(open_only, LOIHI_RX_MEMNAME, read_write);

        // Map the whole shared memory in this process
        mapped_region region(shm, read_write);

        // Get the address of the mapped region
        void *addr = region.get_address();

        // Construct the shared structure in memory
        loihi_rx_shm_data = static_cast<loihi_rx_shm*>(addr);

        std::cout << "[SHM] Linked to Loihi RX shared memory" << std::endl;
    
    } catch(interprocess_exception &ex){
      std::cout << "[SHM] RX Boost interprocess exception: " << ex.what() << std::endl;
      return false;
    }
	
	return true;
}

bool createTXSharedMemory();
bool createTXSharedMemory() {
    try {

        // Create a shared memory object.
        boost::interprocess::shared_memory_object shm(open_only, LOIHI_TX_MEMNAME, read_write);

        // Map the whole shared memory in this process
        mapped_region region(shm, read_write);

        // Get the address of the mapped region
        void *addr = region.get_address();

        // Construct the shared structure in memory
        loihi_tx_shm_data = static_cast<loihi_tx_shm*>(addr);

        std::cout << "[SHM] Linked to Loihi TX shared memory" << std::endl;
    
    } catch(interprocess_exception &ex){
      std::cout << "[SHM] TX Boost interprocess exception: " << ex.what() << std::endl;
      return false;
    }
	
	return true;
}


int main(int argc, char** argv) {

    if (!createRXSharedMemory()) {
		std::cout << "[SHM] Error. Can't link to Loihi RX shared memory" << std::endl;
        return 0;
	}

	if (!createTXSharedMemory()) {
		std::cout << "[SHM] Error. Error. Can't link to Loihi TX shared memory" << std::endl;
        return 0;
	}

    while(1) {



        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return 0;
}

