#include <stdint.h>

int sum(uint8_t* a)
{
  int32_t s = 0;
  for (int i = 0; i < 64; i++) {
    s += a[i];
  }
  return s;
}
