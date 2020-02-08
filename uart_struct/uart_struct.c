#include <stdio.h>
#include <string.h>
#include "uart_struct.h"

#define DEBUG

/* come back to it later
void printstruct(uart_packet_t uart_packet) {
    printf("start_byte: %c \n /
    source_id: %s \n /
    destination_id: %s \n /
    frame_type: %s \n /
    drone_state: 
    end_byte:");
} */

/* return length of packet */
uint8_t struct2packet(uart_packet_t uart_packet, uint8_t* s) {
    
    uint8_t i = 0;
    
    s[i] = uart_packet.start_byte; // 0
    i = i + sizeof(uart_packet.start_byte); 

    s[i] = uart_packet.source_id;  // 1
    i = i + sizeof(uart_packet.source_id); 

    s[i] = uart_packet.destination_id; // 2
    i = i + sizeof(uart_packet.destination_id);
    
    s[i] = uart_packet.frame_type;  // 3
    i = i + sizeof(uart_packet.frame_type);
    
    // opened up struct, point to highest in the array while ieee754 memcpy of float
    // caution: little-endian
    memcpy(&s[i], &uart_packet.drone_state.pos.x, 
            sizeof(uart_packet.drone_state.pos.x)); // 4-7 (7 is MSB)
    i = i + sizeof(uart_packet.drone_state.pos.x); 
    
    memcpy(&s[i], &uart_packet.drone_state.pos.y, 
            sizeof(uart_packet.drone_state.pos.y)); // 8-11 (11 is MSB)
    i = i + sizeof(uart_packet.drone_state.pos.y); 

    memcpy(&s[i], &uart_packet.drone_state.pos.z, 
            sizeof(uart_packet.drone_state.pos.z)); // 12-15 (15 is MSB)
    i = i + sizeof(uart_packet.drone_state.pos.z); 


    memcpy(&s[i], &uart_packet.drone_state.heading, 
            sizeof(uart_packet.drone_state.heading)); // 16-19 (19 is MSB) 
    i = i + sizeof(uart_packet.drone_state.heading); 

    s[i] = uart_packet.end_byte; // 20
    i = i + sizeof(uart_packet.end_byte);

    // TODO: append crc
    
    #ifdef DEBUG
        printf("Packet:");
        for (int cnt = 0; cnt < UNPADDED_LENGTH; cnt++) {
            printf("|0x%02x|", s[cnt]);
        }
        printf("\n");
    #endif

    return i;
}

void packet2struct(uart_packet_t* uart_packet, char *s) {

    // *uart_packet = {.start_byte = c[0], .source_id = c[1], .destination_id = c[2], .frame_type = c[3], .};

}

void main() {
    printf("size of typedef: %ld B\n", sizeof(uart_packet_t));
    uart_packet_t uart_packet = {
        .start_byte = 100,
        .source_id = 2,
        .destination_id = 3,
        .frame_type = 4,
        .drone_state = {
            .pos = {
                .x = 113.523,
                .y = 212.5,
                .z = -9993.5,
            },
        .heading = 1,
        },
        .end_byte = 5,
    };

    char s[MAX_LENGTH] = {0};
    uint8_t len = struct2packet(uart_packet, s);

    // char populate_str[MAX_LENGTH] = {'$', 0, 1, 1, -1.2, -1.2, 1.2, 1.2, '*'};
    // // terminate at struct end
    // populate_str[sizeof(uart_packet_t)] = 0;
    // printf("size of populate_str: %ld B\n", sizeof(populate_str));

    // memcpy(&uart_packet, &populate_str, sizeof(uart_packet_t));
}

/* casting char buffers into struct depends on endian-ness and paddings of the CPU, so it is risky to cast a string buffer to typedefed struct */
/* source, stackoverflow: Mrunmoy */

// char s[MAX_LENGTH] = {0};
// snprintf expects size of s to atleast be sizeof(uart_packet_t) + 1
// '0' + i trick

// /* uint8_t can go directly to uart */
// uint8_t s[UNPADDED_LENGTH] = {0};
// // clear unsigned char array
// memset(s, 0, UNPADDED_LENGTH);

// opened up struct all temp
// all typedef enums get uint8_t