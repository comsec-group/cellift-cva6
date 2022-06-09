// See LICENSE for license details.

#include <stdint.h>
#include "my_string.h"
// #include <stdio.h>

#include "vm.h"
#include "syscall.h"

extern volatile uint64_t* _mstack_end;

# define NPT 4
# define l1pt pt[0]
# define user_l2pt pt[1]
# define kernel_l2pt pt[2]
# define user_llpt pt[3]

__attribute__((section( ".pte" )))
pte_t pt[NPT][PTES_PER_PT] __attribute__((aligned(PGSIZE)));

// typedef struct { pte_t addr; void* next; } freelist_t;

// __attribute__((section( ".bss" )))
// freelist_t user_mapping[MAX_TEST_PAGES];
// __attribute__((section( ".bss" )))
// freelist_t freelist_nodes[MAX_TEST_PAGES];
// __attribute__((section( ".bss" )))
// freelist_t *freelist_head, *freelist_tail;

extern uint64_t *userstart;
extern uint64_t *user_pages;


void error(trapframe_t* tf, int terminate){
    if(terminate)
      while(1);
}

// static inline map_pa2kva(uint64_t paddr){
//   uint64_t vaddr = pa2kva(paddr);
//   VPN1 =    
//   kernel_l2pt[VPN1]
// }

void vm_boot(uintptr_t test_addr)
{
  asm volatile(
    "li t0, 0xFFFFFFFFFFFFFFFF\n"
    "csrw mcounteren, t0\n" //# allows to read all the counters S mode
    "csrw scounteren, t0\n" //# allows to read all the counters U mode
    ); 
  // init_uart(50000000, 115200);

#if (MAX_TEST_PAGES > PTES_PER_PT) || (DRAM_BASE % MEGAPAGE_SIZE) != 0
# error
#endif

  // map the megapage [0xFFFFFFFFFFE00000, 0xFFFFFFFFFFFFFFFF] to [0x80000000, 0x801fffff]
  l1pt[PTES_PER_PT-1] = ((pte_t)kernel_l2pt >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V; // VPN2 = '1, indirection
  // VPN1 = '1, this is already a leaf so it's a megapage, the offset is [VPN0, 12 last bits]
  kernel_l2pt[PTES_PER_PT-1] = (DRAM_BASE/RISCV_PGSIZE << PTE_PPN_SHIFT) | PTE_V | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D; 
  
  // map user to lowermost megapage : VPN2 and VPN1 bits (30th to 38th) of all user pages must be set to 0
  l1pt[0]            = ((pte_t)  user_l2pt  >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V; // VPN2 = '0, indirection to user_l2pt
  user_l2pt[0]       = ((pte_t)  user_llpt  >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V; // VPN1 = '0, indirection to user_llpt

  // here we map the real user pages using pa2uva(pa) = pa - DRAM_BASE 
  // VPN0 = (vaddr >> PGSHIFT) is the index of this array (9 bits -> from 0 to 511=0x1FF) 
  // users VA pages can be from 0x0 to 0x1FF000
  // assert(  ((vaddr >> PGSHIFT) & ~0x1FF) == 0) // ensures VPN2 VPN1 are 0 

  // Set the user instruction page.
  user_llpt[(uint64_t)pa2uva((void*)&userstart) >> PGSHIFT] = ((uint64_t)&userstart >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V | PTE_U | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D; 
  // Set the user data page.
  user_llpt[(uint64_t)pa2uva((void*)&user_pages) >> PGSHIFT] = ((uint64_t)&user_pages >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V | PTE_U | PTE_R | PTE_W | PTE_X | PTE_A | PTE_D; 

  // set satp register (previously called sptbr)
  uintptr_t vm_choice = SATP_MODE_CHOICE;
  uintptr_t sptbr_value = ((uintptr_t)l1pt >> PGSHIFT)
                        | (vm_choice * (SATP_MODE & ~(SATP_MODE<<1)));
  write_csr(sptbr, sptbr_value);
  // if (read_csr(sptbr) != sptbr_value)
  //   assert(!"unsupported satp mode");

  // Set up PMPs if present, ignoring illegal instruction trap if not.
  uintptr_t pmpc = PMP_NAPOT | PMP_R | PMP_W | PMP_X;
  uintptr_t pmpa = ((uintptr_t)1 << (__riscv_xlen == 32 ? 31 : 53)) - 1;
  asm volatile ("la t0, 1f\n\t"
                "csrrw t0, mtvec, t0\n\t"
                "csrw pmpaddr0, %1\n\t"
                "csrw pmpcfg0, %0\n\t"
                ".align 2\n\t"
                "1: csrw mtvec, t0"
                : : "r" (pmpc), "r" (pmpa) : "t0");

  // set up supervisor trap handling
  write_csr(stvec, pa2kva(trap_entry));
  write_csr(sscratch, pa2kva(read_csr(mscratch)));
  write_csr(mscratch,&_mstack_end);

  write_csr(medeleg,
    (1 << CAUSE_USER_ECALL) |
    (1 << CAUSE_FETCH_PAGE_FAULT) |
    (1 << CAUSE_LOAD_PAGE_FAULT) |
    (1 << CAUSE_STORE_PAGE_FAULT));

  // FPU on; accelerator on; vector unit on
  write_csr(mstatus, MSTATUS_FS | MSTATUS_XS | MSTATUS_VS);
  write_csr(mie, 0);

  // freelist_head = pa2kva((void*)&freelist_nodes[0]);
  // freelist_tail = pa2kva(&freelist_nodes[MAX_TEST_PAGES-1]);

  // for (long i = 1; i < MAX_TEST_PAGES; i++)
  // {
  //   freelist_nodes[i].addr = (uint64_t)&user_pages + i*PGSIZE;
  //   freelist_nodes[i].next = pa2kva(&freelist_nodes[i+1]);
  // }

  // freelist_nodes[MAX_TEST_PAGES-1].next = 0;

  // memset(user_mapping, 0, sizeof(user_mapping));
  asm volatile ("fence.i");

  trapframe_t tf;
  memset(&tf, 0, sizeof(tf));

  // this setup the user address space 
  //tf.epc = test_addr - DRAM_BASE;
  tf.epc = (uint64_t)pa2uva(test_addr);
  pop_tf(&tf);
}

void handle_trap_M(trapframe_t* tf)
{
  // here we are in M mode 
  if (tf->cause == CAUSE_SUPERVISOR_ECALL || tf->cause == CAUSE_MACHINE_ECALL)
  {
    tf->epc += 4;
    
  }
  else if (tf->cause == CAUSE_ILLEGAL_INSTRUCTION)
  {
    error(tf, 0);
    tf->epc += 4;
  }
  
  else {
  //  uart_puts("unexpected exception\r\n");
   error(tf, 1);
  }

  pop_tf_M(tf);
  
}

