/*
    author: nilay994
    Program to (a) make char arrays from a struct and transfer them over serial (b) populate struct from rx_buffer. Uses "LSB first".
    Stefano: Keep in mind that ARM CPUs (at least up to ARM7 and 9) swap the 4 least significant bytes with the most significant ones in memory for double precision floats (with respect to Intel CPUs…) so a memcpy can give you quite a headache…
    One of the simple approaches might be typecasting but keep an eye on the byte order. A stupid solution might be creating an union between a float and an array of 4 bytes. You can use both notations and perform any order swap you want in a very easy way. 
*/


#include <stdint.h>

#define MAX_LENGTH 50
#define UNPADDED_LENGTH 21

typedef enum {
    ESP,
    BEBOP
} device_t;

typedef enum {
    ACK_FRAME,
    DATA_FRAME
} frame_t; 


// uint32_t hold hex for ieee754 float
typedef struct {
    float x;     // lat
    float y;     // long
    float z;     // alt
} vec3f_t;

typedef struct {
    vec3f_t pos;
    float heading;
    // vec3f_t vel;
    // vec3f_t att;
    // vec3f_t omega;
} drone_state_t;

// compiler padding is on by default, expect size larger than sum
typedef struct {
    uint8_t start_byte;
    uint8_t source_id;
    uint8_t destination_id;
    uint8_t frame_type;
    drone_state_t drone_state;
    uint8_t end_byte;
} uart_packet_t;

typedef struct __attribute__((packed)) {
    uint8_t nibble1: 4;
    uint8_t nibble2: 4;
    uint32_t floatval;
    uint8_t charval;
} fragged_packet_t;

//__attribute__((packed));