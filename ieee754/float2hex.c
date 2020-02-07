/* stack-overflow comment. Not sure if this is ieee754 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main() {
  uint32_t fi = 0x3f520000;
  float f;
  memcpy(&f, &fi, sizeof(f));
  printf("%.7g\n", f);
}
