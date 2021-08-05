#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
  int i=0; 
  while(i<32){
    printf("%-3s : 0x%lx\n",regs[i],reg_d(i));
    i++;
  }
  printf("pc  : 0x%lx\n",cpu.pc);
}

uint64_t isa_reg_str2val(const char *s, bool *success) {
  int i=0;
  uint64_t val=0;
  // printf("isa_reg_str2val %s\n",s);
  // char * ss=0;
  // s++;
  // while(*s){
  //   *(ss++) = *(s++);
  // }
  // *ss = '\0';
  // printf("isa_reg_str2val %s\n",ss);
  while(i<32){
    if(strcmp(s,regs[i])==0){
      *success = true;
      val = reg_d(i);
    }
    i++;
  }

  //if(*success) printf("success\n");
  return val;
}
