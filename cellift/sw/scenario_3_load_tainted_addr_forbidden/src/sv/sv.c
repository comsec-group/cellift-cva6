// See LICENSE for license details.

#include <stdint.h>
#include "my_string.h"

#include "vm.h"
#include "syscall.h"

//if SATP_MODE_CHOICE == SATP_MODE_SV39
# define NPT 4
# define user_llpt pt[3]
// typedef struct { pte_t addr; void* next; } freelist_t;

// extern pte_t pt[NPT][PTES_PER_PT];
// extern freelist_t user_mapping[MAX_TEST_PAGES];
// extern freelist_t freelist_nodes[MAX_TEST_PAGES];
// extern freelist_t *freelist_head, *freelist_tail;

// static void evict(unsigned long addr)
// {
//   //assert(addr >= PGSIZE && addr < MAX_TEST_PAGES * PGSIZE);


//   addr = addr/PGSIZE*PGSIZE;

//   freelist_t* node = &user_mapping[addr/PGSIZE];
//   if (node->addr)
//   {
//     // check accessed and dirty bits
//     //assert(user_llpt[addr/PGSIZE] & PTE_A);
//     uintptr_t sstatus = set_csr(sstatus, SSTATUS_SUM);
//     if (memcmp((void*)addr, uva2kva(addr), PGSIZE)) {
//       //assert(user_llpt[addr/PGSIZE] & PTE_D);
//       memcpy((void*)addr, uva2kva(addr), PGSIZE);
//     }
//     write_csr(sstatus, sstatus);

//     user_mapping[addr/PGSIZE].addr = 0;

//     if (freelist_tail == 0)
//       freelist_head = freelist_tail = node;
//     else
//     {
//       freelist_tail->next = node;
//       freelist_tail = node;
//     }
//   }
// }

// void handle_fault(uintptr_t addr, uintptr_t cause)
// {
//   addr = addr/PGSIZE*PGSIZE;

//   // if the page is already in the page table 
//   if (user_llpt[addr/PGSIZE]) {

//     // set the access bit if not already set
//     if (!(user_llpt[addr/PGSIZE] & PTE_A)) {
//       user_llpt[addr/PGSIZE] |= PTE_A;
//     }

//     // if the PF is from a store, set the dirty bit
//     if(!(user_llpt[addr/PGSIZE] & PTE_D) && cause == CAUSE_STORE_PAGE_FAULT){
//       user_llpt[addr/PGSIZE] |= PTE_D;
//       //print_uart_addr(user_llpt[addr/PGSIZE]);
//     }
    
//     // flush the page from the TLB 
//     flush_page(addr);
//     asm volatile ("fence.i");

//     return;
//   }

//   freelist_t* node = freelist_head;
//   if(!node){
//     // terminate
//     while(1);
//   }
//   freelist_head = node->next;
//   if (freelist_head == freelist_tail)
//     freelist_tail = 0;

//   // if the page is not in the page table, create a new leaf entry (A and D bits set anyway) 
//   uintptr_t new_pte = (node->addr >> PGSHIFT << PTE_PPN_SHIFT) | PTE_V | PTE_U | PTE_R | PTE_W | PTE_X;
//   user_llpt[addr/PGSIZE] = new_pte | PTE_A | PTE_D;

// //  printf("%c-PF at the VA: 0x%08x --> now mapped to PA: 0x%05x000\r\n", (cause == CAUSE_STORE_PAGE_FAULT) ? 'S' : (cause == CAUSE_LOAD_PAGE_FAULT) ? 'L' : 'I', addr, user_llpt[addr/PGSIZE]>>10);

  
//   flush_page(addr);

//   // user_mapping keeps track of the translations
//   if(user_mapping[addr/PGSIZE].addr != 0){
//     // printf("problem : user_mapping[0x%x].addr already written with 0x%08x\r\n", addr/PGSIZE, user_mapping[addr/PGSIZE].addr);
//   }
//   user_mapping[addr/PGSIZE] = *node;


//   // allows SV mode to access user pages 
//   uintptr_t sstatus = set_csr(sstatus, SSTATUS_SUM);
//   // copy the physical page to the new mapped page
//   memcpy((void*)addr, uva2kva(addr), PGSIZE);
//   write_csr(sstatus, sstatus);

//   flush_page(addr);
//   asm volatile ("fence.i");
// }

// void handle_trap(trapframe_t* tf)
// {

//   if (tf->cause == CAUSE_USER_ECALL) {
    
//     int fcode = tf->gpr[11];

//     switch(fcode){
//       case 'c':
//         _putchar((char)(uint64_t)uva2kva(tf->gpr[10]));
//         break;
//       case 0xF:
//         flush_page(tf->gpr[10]);
//         break;
//       default:
//         // printf("Unknown user syscall\n");
//         break;
//     }
//     tf->epc += 4;
//   }
//   else if (tf->cause == CAUSE_ILLEGAL_INSTRUCTION)
//   {
//     //assert(tf->epc % 4 == 0);

//     int* fssr;
//     asm ("jal %0, 1f; fssr x0; 1:" : "=r"(fssr));

//     // if (*(int*)tf->epc == *fssr)
//     //   terminate(1); // FP test on non-FP hardware.  "succeed."
//     // else
//     //   assert(!"illegal instruction");
//     tf->epc += 4;
//   }
//   else if (tf->cause == CAUSE_FETCH_PAGE_FAULT || tf->cause == CAUSE_LOAD_PAGE_FAULT || tf->cause == CAUSE_STORE_PAGE_FAULT){
//     handle_fault(tf->badvaddr, tf->cause);
//   }
//   else
//     // printf("unexpected exception");
  
//   pop_tf(tf);
// }
