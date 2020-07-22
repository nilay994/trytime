/*
    author: fedepare
    Program to read and write shared memory with the uart_driver module. 
*/

#pragma once 

#include <stdint.h>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

struct loihi_rx_shm {

   loihi_rx_shm()
      :  divergence(0)
      ,  divergence_dot(0)
      ,  flag(false)
   {}

   // Mutex to protect access to the queue
   boost::interprocess::interprocess_mutex mutex;

   // Items to fill
   int cnt;
   float divergence;
   float divergence_dot;
   bool flag;
};

struct loihi_tx_shm {

   loihi_tx_shm()
      :  thurst(0)
      ,  flag(false)
   {}

   // Mutex to protect access to the queue
   boost::interprocess::interprocess_mutex mutex;

   // Items to fill
   float thurst;
   bool flag;
};