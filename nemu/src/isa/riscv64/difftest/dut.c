#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

extern const char *regs[];
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool flag = true;
  if(!difftest_check_reg("pc", pc, ref_r->pc, cpu.pc))
    flag = false;
  for(int i=0; i<32; ++i){
    if(!difftest_check_reg(regs[i], pc, ref_r->gpr[i]._64, cpu.gpr[i]._64))
      flag = false;
  }
  return flag;
}

void isa_difftest_attach() {
}
