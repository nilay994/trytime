#include <stdio.h>
#include <string.h>
#include "uart_struct.h"

#define DEBUG


void printpacketscreen(char *s) {
    static int i = 0;
    printf("||");
    while (*s!=0 || *s!='\n' || *s!='\r' || *s!='\0') {
        printf("%c", s[i]);
        i = i + 1;
    }
    i = 0;
    printf("||\n");
}

/* come back to it later
void printstruct(uart_packet_t uart_packet) {
    printf("start_byte: %c \n /
    source_id: %s \n /
    destination_id: %s \n /
    frame_type: %s \n /
    drone_state: 
    end_byte:");
} */

void struct2packet(uart_packet_t uart_packet) {
    // char s[MAX_LENGTH] = {0};
    // snprintf expects size of s to atleast be sizeof(uart_packet_t) + 1
    // '0' + i trick
    // uint8_t can go directly to uart
    int i = UNPADDED_LENGTH;
    char s[50] = {0};
    // clear char array
    memset(s, 0, UNPADDED_LENGTH);
    
    // opened up struct all temp
    uint8_t uart_packet_start_byte = uart_packet.start_byte;
    i = i - sizeof(uart_packet.start_byte);
    s[i] = uart_packet_start_byte; // 20

    // all typedef enums get uint8_t
    uint8_t uart_packet_source_id = uart_packet.source_id;
    i = i - sizeof(uart_packet.source_id); 
    s[i] = uart_packet_source_id;  // 19
    
    uint8_t uart_packet_destination_id = uart_packet.destination_id;
    i = i - sizeof(uart_packet.destination_id);
    s[i] = uart_packet_destination_id; // 18

    uint8_t uart_packet_frame_type = uart_packet.frame_type;
    i = i - sizeof(uart_packet.frame_type);
    s[i] = uart_packet_frame_type;  // 17

    // opened up struct, highest in the array = sent first, ieee754 when memcpy of float
    uint8_t uart_packet_drone_state_pos_x[4];
    i = i - sizeof(uart_packet.drone_state.pos.x); 
    memcpy(&s[i], &uart_packet.drone_state.pos.x, sizeof(uart_packet.drone_state.pos.x)); // 13-16
    

    uint8_t uart_packet_drone_state_pos_y[4];
    i = i - sizeof(uart_packet.drone_state.pos.y); 
    memcpy(&s[i], &uart_packet.drone_state.pos.y, sizeof(uart_packet.drone_state.pos.y)); // 9-12

    uint8_t uart_packet_drone_state_pos_z[4];
    i = i - sizeof(uart_packet.drone_state.pos.z); 
    memcpy(&s[i], &uart_packet.drone_state.pos.z, sizeof(uart_packet.drone_state.pos.z)); // 5-8 

    uint8_t uart_packet_drone_state_heading[4];
    i = i - sizeof(uart_packet.drone_state.heading); 
    memcpy(&s[i], &uart_packet.drone_state.heading, sizeof(uart_packet.drone_state.heading)); // 1-4 

    uint8_t uart_packet_end_byte = uart_packet.end_byte; // 0

    char tmp[UNPADDED_LENGTH+1] = {0};
    snprintf(tmp, 21, "%s", s);
    printf("\n||%s||\n", tmp);

    #ifdef DEBUG
        // printpacketscreen(s);
    #endif
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
                .x = 1.5,
                .y = 2.5,
                .z = 3.5,
            },
        .heading = -350,
        },
        .end_byte = 5 
    };
    char *s[MAX_LENGTH] = {0};
    struct2packet(uart_packet);

    // char populate_str[MAX_LENGTH] = {'$', 0, 1, 1, -1.2, -1.2, 1.2, 1.2, '*'};
    // // terminate at struct end
    // populate_str[sizeof(uart_packet_t)] = 0;
    // printf("size of populate_str: %ld B\n", sizeof(populate_str));

    // memcpy(&uart_packet, &populate_str, sizeof(uart_packet_t));
}

/* casting char buffers into struct depends on endian-ness and paddings of the CPU, so it is risky to cast a string buffer to typedefed struct */
/* source, stackoverflow: Mrunmoy */