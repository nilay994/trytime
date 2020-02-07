/*
 *
 *
 *  Created on: 8 January 2019
 *  Author: Andrea Ricchetti
 */

#include "ieee754.h"
#include <stdio.h>


typedef union UnFloatingPointIEEE754
{
  struct
  {
    unsigned int mantissa : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } raw;

  float f;

} UFloatingPointIEEE754;


#define NTH_BIT(b, n) ((b >> n) & 0x1)

#define BYTE_TO_BIN(b)    (( b & 0x80 ) ) |\
                          (( b & 0x40 ) ) |\
                          (( b & 0x20 ) ) |\
                          (( b & 0x10 ) ) |\
                          (( b & 0x08 ) ) |\
                          (( b & 0x04 ) ) |\
                          (( b & 0x02 ) ) |\
                          ( b & 0x01 )

#define MANTISSA_TO_BIN(b)  (( b & 0x400000 ) ) |\
                            (( b & 0x200000 ) ) |\
                            (( b & 0x100000 ) ) |\
                            (( b &  0x80000 ) ) |\
                            (( b &  0x40000 ) ) |\
                            (( b &  0x20000 ) ) |\
                            (( b &  0x10000 ) ) |\
                            (( b &  0x8000 ) ) |\
                            (( b &  0x4000 ) ) |\
                            (( b &  0x2000 ) ) |\
                            (( b &  0x1000 ) ) |\
                            (( b &  0x800 ) ) |\
                            (( b &  0x400 ) ) |\
                            (( b &  0x200 ) ) |\
                            (( b &  0x100 ) ) |\
                            (( b &  0x80 ) ) |\
                            (( b &  0x40 ) ) |\
                            (( b &  0x20 ) ) |\
                            (( b &  0x10 ) ) |\
                            (( b &  0x08 ) ) |\
                            (( b &  0x04 ) ) |\
                            (( b &  0x02 ) ) |\
                            ( b & 0x01 )



uint32_t pack754_32( float f )
{
  UFloatingPointIEEE754 ieee754;
  uint32_t    floatingToIntValue = 0;

  ieee754.f = f;
  // Create uint32_t value from float value

  floatingToIntValue = (((NTH_BIT(ieee754.raw.sign, 0) << 8) | (BYTE_TO_BIN(ieee754.raw.exponent))) << 23 ) | MANTISSA_TO_BIN(ieee754.raw.mantissa);

  return floatingToIntValue;
}

float unpack754_32( uint32_t floatingToIntValue )
{
  UFloatingPointIEEE754 ieee754;

  unsigned int mantissa = 0;
  unsigned int exponent = 0 ;
  unsigned int sign = 0;

  sign = NTH_BIT(floatingToIntValue, 31);
  for( int ix=0; ix<8; ix++)
   exponent = (exponent | (NTH_BIT(floatingToIntValue, (30-ix))))<<1;
  exponent = exponent>>1;
  for( int ix=0; ix<23; ix++)
   mantissa = (mantissa | (NTH_BIT(floatingToIntValue, (22-ix))))<<1;
  mantissa = mantissa >> 1;

  ieee754.raw.sign = sign;
  ieee754.raw.exponent = exponent;
  ieee754.raw.mantissa = mantissa;

  return ieee754.f;
}

void main ( void )
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
