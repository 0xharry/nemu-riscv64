#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc) {
// 将当前PC值保存到sepc寄存器--pc
  cpu.csr.sepc = cpu.pc;
// 在 scause 寄存器中设置异常号--gpr(a7)
  cpu.csr.scause = NO; // cpu.gpr[17]._64;
// 从 stvec 寄存器中取出异常入口地址
// 跳转到异常入口地址-- j (stvec)
// #ifdef DEBUG
  Log("cpu.csr.sepc=%p",(void*)cpu.csr.sepc);
// #endif
  rtl_j(s, cpu.csr.stvec);
}

void query_intr(DecodeExecState *s) {
}
