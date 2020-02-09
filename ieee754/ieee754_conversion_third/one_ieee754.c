/*
 *
 *
 *  Created on: 8 January 2019
 *  Author: Andrea Ricchetti
 */

#include "ieee754.h"
#include <stdio.h>

// use memcpy instead
uint32_t pack754_32( float f )
{
  uint32_t    *pfloatingToIntValue;
  pfloatingToIntValue = &f;

  return (*pfloatingToIntValue);

}

float unpack754_32( uint32_t floatingToIntValue )
{
  float *pf, f;
  pf = &(floatingToIntValue);
  f = *pf;

  return f;
}

void main( void )
{
  uint32_t n;
  float f;

  n = 0x3FB4FDF4;   //1068826100 -> 1.414
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(1.414);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(-1.259921);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(0.58);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(-0.588);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(2);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

  n = pack754_32(-3);
  f = unpack754_32(n);
  printf("0x%06x, %f\n", n, f);

}
