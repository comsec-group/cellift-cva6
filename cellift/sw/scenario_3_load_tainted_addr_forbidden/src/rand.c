#include "rand.h"
#include <stdint.h>

uint64_t lfsr63(uint64_t x)
{
  uint64_t bit = (x ^ (x >> 1)) & 1;
  return (x >> 1) | (bit << 62);
}

static inline uint64_t rdtime() {
  uint64_t rv;
  asm volatile ("rdcycle %0": "=r" (rv) ::);
  return rv;
}

static unsigned long int next = 1;

int rand(void) // RAND_MAX assumed to be 32767
{
    next = next * 1103515245 + 12345;
    return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed)
{
    next = seed;
}

uint32_t random(void) {
   srand(rdtime());
   return rand();
}