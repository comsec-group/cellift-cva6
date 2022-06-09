OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  RAM_START = 0x00000000;
  . = RAM_START;
  .text.init : { *(.text.init) }
  . = RAM_START + 0xa000;
  tainted_symbol = .;
}
