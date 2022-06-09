
  .section ".text.init","ax",@progbits
  .globl _start
  .align 2
_start:
    lw t0, tainted_symbol
infinite_loop:
    j infinite_loop
