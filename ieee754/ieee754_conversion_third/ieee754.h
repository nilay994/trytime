/*
 *
 *
 *  Created on: 8 January 2019
 *  Author: Andrea Ricchetti
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __math_utility_functions_H_
#define __math_utility_functions_H_
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>
/* Includes ------------------------------------------------------------------*/

uint32_t pack754_32(float f);
float unpack754_32(uint32_t floatingToIntValue);

// purpose test only
void TestPackUnpack ( void );

#ifdef __cplusplus
}
#endif
#endif /*__ __math_utility_functions_H_ */

/**
  * @}
  */

/**
  * @}
  */

