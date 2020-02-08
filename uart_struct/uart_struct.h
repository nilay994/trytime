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

// __attribute__((packed));