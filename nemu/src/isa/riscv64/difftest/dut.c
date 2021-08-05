#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"
#include <memory/paddr.h>

extern bool difftest_check_reg(const char *name, vaddr_t pc, rtlreg_t ref, rtlreg_t dut);

//difftest_check_reg(const char *name, vaddr_t pc, rtlreg_t ref, rtlreg_t dut)
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  int i=1;
  bool flag = true;

  //0号寄存器永远为0，而且每次运行完都需要reset，比较没有意义
  for(i=1;i<32;i++){
    if(difftest_check_reg(reg_name(i),pc,ref_r->gpr[i]._64,reg_d(i))==false)
      flag = false;
  }

  if(difftest_check_reg("pc",pc,ref_r->pc,cpu.pc)==false) flag = false;
  return flag;
}

//???
void isa_difftest_attach() {
}
