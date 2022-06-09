#include "syscall.h"
#include <stdint.h>


void syscall(void* arg, uint8_t fcode) {
  asm volatile(
    "mv a1, %0\n" // prevent compiler optimization
    "ecall\n"
    ::"r"(fcode):
  );
}

// same syscall for SV and U mode 
void _putchar(char c)
{
  syscall((void*)((uint64_t)c), 'c');
}


