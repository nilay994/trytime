#ifndef PRODUCER_H
#define PRODUCER_H

#include <stdint.h>
#include "RingBuffer.hpp"

void producer(RingBuffer<uint8_t, 20> * rb);

#endif /* PRODUCER_H */