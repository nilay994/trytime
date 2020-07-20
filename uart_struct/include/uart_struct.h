/*
    author: nilay994
    Program to (a) make char arrays from a struct and transfer them over serial (b) populate struct from rx_buffer. Uses "LSB first".
    Stefano: Keep in mind that ARM CPUs (at least up to ARM7 and 9) swap the 4 least significant bytes with the most significant ones in memory for double precision floats (with respect to Intel CPUs…) so a memcpy can give you quite a headache…
    One of the simple approaches might be typecasting but keep an eye on the byte order. A stupid solution might be creating an union between a float and an array of 4 bytes. You can use both notations and perform any order swap you want in a very easy way. 
*/

#pragma once 

#include <stdint.h>

/* typedef enum {
    ESP,
    BEBOP
} device_t;

typedef enum {
    ACK_FRAME,
    DATA_FRAME
} frame_t; 

// compiler padding is on by default, expect size larger than sum
typedef struct {
    uint8_t start_byte;
    uint8_t source_id;
    uint8_t destination_id;
    uint8_t frame_type;
    drone_state_t drone_state;
    uint8_t end_byte;
} uart_packet_t; */

typedef struct __attribute__((packed)) {
  float pot;
  uint8_t but0:1;
  uint8_t but1:1;
  uint8_t but2:1;
  uint8_t but3:1;
  uint8_t but4:1;
} input_dev_t;
