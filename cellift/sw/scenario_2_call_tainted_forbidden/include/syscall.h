#pragma once

#include <stdint.h> 

void syscall(void* arg, uint8_t fcode);
void _putchar(char character);