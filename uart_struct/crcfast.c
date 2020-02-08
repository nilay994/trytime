#include <stdint.h>
#include <stdio.h>
#include "crcfast.h"

/*------------------------------------------------------------------
 * crcInit -- Creates a table for quick crc-8 lookup
 * Parameters:			Void pointer to parametres (unused)
 * Author:	Niels Hokke
 *------------------------------------------------------------------
 */
void crc_init(void){
    uint8_t  remainder;
    for (int dividend = 0; dividend < 256; ++dividend)
    {
        remainder = dividend << (WIDTH - 8);
        for (uint8_t bit = 8; bit > 0; --bit){
            if (remainder & TOPBIT){
                remainder = (remainder << 1) ^ POLYNOMIAL;
            }else{
                remainder = (remainder << 1);
            }
        }
        crcTable[dividend] = remainder;
    }
}


/*------------------------------------------------------------------
 * crcInit -- Calculates crc of a message
 * Parameters:			char message[], char array containing message
 *						int nBytes, size of message
 * Author:	Niels Hokke
 *------------------------------------------------------------------
 */
uint8_t crc_fast(char message[], int nBytes){
    uint8_t data;
    uint8_t remainder = 0;

    for (int byte = 0; byte < nBytes; ++byte){
        data = message[byte] ^ (remainder >> (WIDTH - 8));
        remainder = crcTable[data] ^ (remainder << 8);
    }
    return (remainder);
}


void main() {

}
