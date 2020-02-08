#include <stdio.h>
#include <string.h>
#include "uart_struct.h"

#define DEBUG

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

    // TODO: append crc or terminate
    
    #ifdef DEBUG
        printf("Packet:{");
        for (int cnt = 0; cnt < UNPADDED_LENGTH; cnt++) {
            printf("0x%02x,", s[cnt]);
        }
        printf("}\n");
    #endif

    return i;
}

uint8_t packet2struct(uart_packet_t* uart_packet, uint8_t *s) {

    // endian-ness solved but padding is a problem: memcpy(uart_packet, &s, sizeof(uart_packet_t));

    uint8_t i = 0;
    memcpy(&uart_packet->start_byte, &s[i], sizeof(uart_packet->start_byte));
    i = i + sizeof(uart_packet->start_byte);

    memcpy(&uart_packet->source_id, &s[i], sizeof(uart_packet->start_byte));
    i = i + sizeof(uart_packet->source_id);

    memcpy(&uart_packet->destination_id, &s[i], sizeof(uart_packet->start_byte));
    i = i + sizeof(uart_packet->destination_id);

    memcpy(&uart_packet->frame_type, &s[i], sizeof(uart_packet->drone_state));
    i = i + sizeof(uart_packet->frame_type);

    // caution: little-endian, memcpy start block lower address
    memcpy(&uart_packet->drone_state.pos.x, &s[i], sizeof(uart_packet->drone_state.pos.x));
    i = i + sizeof(uart_packet->drone_state.pos.x);

    memcpy(&uart_packet->drone_state.pos.y, &s[i], sizeof(uart_packet->drone_state.pos.y));
    i = i + sizeof(uart_packet->drone_state.pos.y);

    memcpy(&uart_packet->drone_state.pos.z, &s[i], sizeof(uart_packet->drone_state.pos.z));
    i = i + sizeof(uart_packet->drone_state.pos.z);

    memcpy(&uart_packet->drone_state.heading, &s[i], sizeof(uart_packet->drone_state.heading));
    i = i + sizeof(uart_packet->drone_state.heading);

    memcpy(&uart_packet->end_byte, &s[i], sizeof(uart_packet->end_byte));
    i = i + sizeof(uart_packet->end_byte);

    #ifdef DEBUG
        printf("uart_packet->start_byte: %d\n \
        uart_packet->source_id: %d\n \
        uart_packet->destination_id: %d\n \
        uart_packet->frame_type: %d\n \
        uart_packet->drone_state.pos.x: %f\n \
        uart_packet->drone_state.pos.y: %f\n \
        uart_packet->drone_state.pos.z: %f\n \
        uart_packet->drone_state.heading: %f\n \
        uart_packet->end_byte: %d\n",
        uart_packet->start_byte,
        uart_packet->source_id,
        uart_packet->destination_id,
        uart_packet->frame_type,
        uart_packet->drone_state.pos.x,
        uart_packet->drone_state.pos.y,
        uart_packet->drone_state.pos.z,
        uart_packet->drone_state.heading,
        uart_packet->end_byte);
    #endif


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

    //char str[21] = {0x64,0x02,0x03,0x04,0xc7,0x0b,0xe3,0x42,0x00,0x80,0x54,0x43,0x00,0x26,0x1c,0xc6,0x00,0x00,0x80,0x3f,0x05};

    char s[MAX_LENGTH] = {0};
    uint8_t len = struct2packet(uart_packet, s);
    printf("stringlen: %d\n", len);

    uart_packet_t uart_packet2;
    packet2struct(&uart_packet2, s);
}