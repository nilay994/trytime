/* casting char buffers into struct depends on endian-ness and paddings of the CPU, so it is risky to use the below */
/* source, stackoverflow: Mrunmoy */
#define <stdio.h>
#define <stdint.h>

typedef struct uart_protocol_t
{
    uint8_t delim0;
    uint8_t interface;
    union {
        struct {
            uint8_t command_H;
            uint8_t command_L;
        } cmd_bytes;
        uint16_t command;
    } cmd_union;
    union {
        struct {
            uint8_t length_H;
            uint8_t length_L;
        } len_bytes;
        uint16_t length;
    } len_union;
    uint8_t payload;
    uint8_t checksum;
    uint8_t delim1;
} uart_protocol_t;

uart_protocol_t *p_proto = (uart_protocol_t *)buffer;
