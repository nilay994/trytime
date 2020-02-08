// defines for CRC calcuation
#define WIDTH  (8 * sizeof(uint8_t))
#define TOPBIT (1 << (WIDTH - 1))
#define POLYNOMIAL 0xD8  /* 11011 followed by 0's */

// CRC lookup table
uint8_t  crcTable[256];
