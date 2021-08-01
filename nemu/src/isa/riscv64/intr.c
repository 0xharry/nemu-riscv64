#include <cpu/exec.h>
#include "local-include/rtl.h"

void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
// 将当前PC值保存到sepc寄存器--pc
// 在 scause 寄存器中设置异常号--NO
// 从 stvec 寄存器中取出异常入口地址
// 跳转到异常入口地址-- j (stvec)

  // rtl_j(s, );
  TODO();
}

void query_intr(DecodeExecState *s) {
}
