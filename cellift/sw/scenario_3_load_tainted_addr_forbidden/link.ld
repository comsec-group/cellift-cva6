OUTPUT_ARCH( "riscv" )
ENTRY(_start)

SECTIONS
{
  . = 0x00000000;
  .text.init : { *(.text.init) }
  . = ALIGN(0x1000);  .text : { *(.text) }
  . = ALIGN(0x1000);
  .data : { *(.data) }
  .bss : { *(.bss) }
  . = ALIGN(0x1000);
  . += 0x1000;
  _sstack_end = .;
  . += 0x1000;
  _mstack_end = .;
  . += 0x1000;
  _ustack_end = .;
  . += 0x1000;
  .user_pages_start : { *(.user_pages_start) }
  . += 0x10;
  . = ALIGN(0x1000);
  _user_pages_end = .;
  .userstart_section : { *(.userstart_section) }
  . += 0x10;
}
