/*
    author: nilay994
    Program to (a) make char arrays from a struct and transfer them over serial (b) populate struct from rx_buffer. Uses "LSB first".
    Stefano: Keep in mind that ARM CPUs (at least up to ARM7 and 9) swap the 4 least significant bytes with the most significant ones in memory for double precision floats (with respect to Intel CPUs…) so a memcpy can give you quite a headache…
    One of the simple approaches might be typecasting but keep an eye on the byte order. A stupid solution might be creating an union between a float and an array of 4 bytes. You can use both notations and perform any order swap you want in a very easy way. 
*/

#pragma once 

#include <stdint.h>

#define UART_MAX_LEN 30

typedef enum {
  ACK_FRAME = 0,
  DATA_FRAME,
  OTHER_FRAME
} frame_type_t;

// decoder state
typedef enum {
  JTSN_SYNC = 0,
  JTSN_INFO,
  JTSN_DATA,
  JTSN_ERR_CHK,  // TODO: shift to crc
  JTSN_RX_OK,
  JTSN_RX_ERR
} jetson_state_t;

typedef struct __attribute__((packed)) {
  float pot;
  uint8_t but0:1;
  uint8_t but1:1;
  uint8_t but2:1;
  uint8_t but3:1;
  uint8_t but4:1;
} data_frame_t;

typedef struct __attribute__((packed)) {
  uint8_t packet_type;
  // packet_length is counted from start byte to the end of data frame
  // in this case: 1 start byte + 2 info bytes + 5 byte of data = 8 bytes
  uint8_t packet_length; 
} info_frame_t;


typedef struct __attribute__((packed)) {
  info_frame_t info;
  data_frame_t data;
} uart_packet_t;

