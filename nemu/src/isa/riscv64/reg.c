#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  printf("cpu.pc = %p\nsepc  = %p\tscause  = %lu\nstvec = %p\tsstatus = %u\n",
          (void*)cpu.pc, (void*)cpu.csr.sepc, cpu.csr.scause, (void*)cpu.csr.stvec, cpu.csr.sstatus.val);
  for(int i=0; i<32; ++i)
    printf("| %d\t%s\t0x%16lx |\n", i,regs[i],cpu.gpr[i]._64);
}

word_t isa_reg_str2val(const char *s, bool *success) {
  if(strncmp("pc", s, 2)==0) {
    *success = true;
    return cpu.pc;
  }

  for(int i=0; i<32; ++i) {
    if(strncmp(regs[i], s, 3)==0) {
      *success = true;
      return cpu.gpr[i]._64;
    }
  }
  
  *success = false;
  return 0;
}
