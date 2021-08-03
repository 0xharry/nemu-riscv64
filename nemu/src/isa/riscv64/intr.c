#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc) {
// 将当前PC值保存到sepc寄存器--pc
  cpu.csr.sepc = cpu.pc;
// 在 scause 寄存器中设置异常号--NO
  cpu.csr.scause = NO;
// 从 stvec 寄存器中取出异常入口地址
// 跳转到异常入口地址-- j (stvec)
  Log("cpu.csr.scause=%lu\tcpu.csr.sepc=%p",cpu.csr.scause,(void*)cpu.csr.sepc);
  rtl_j(s, cpu.csr.stvec);
  rtl_j(s, cpu.csr.sepc+4);
}

void query_intr(DecodeExecState *s) {
}
