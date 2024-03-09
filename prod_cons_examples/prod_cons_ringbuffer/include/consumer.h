#ifndef CONSUMER_H
#define CONSUMER_H

#include <stdint.h>
#include "RingBuffer.hpp"

// TODO: remove the 20? why does it care
void consumer(RingBuffer<uint8_t, 20> * rb);

#endif /* CONSUMER_H */