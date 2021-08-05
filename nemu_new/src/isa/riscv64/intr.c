#include <cpu/exec.h>
#include "local-include/rtl.h"


/*
将当前PC值保存到sepc寄存器
在scause寄存器中设置异常号
从stvec寄存器中取出异常入口地址
跳转到异常入口地址

根据手册extend\
把 sstatus CSR 中的 SIE 置零，屏蔽中断，且 SIE 之前的值被保存在 SPIE 中。
这里只用到了S-mode，所以spp不用设置
*/
void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */
  //printf("get in raise_intr\n");
  cpu.sepc   = epc;
  cpu.scause = NO;
  cpu.sstatus.spie = cpu.sstatus.sie;
  cpu.sstatus.sie  = 0;
  //printf("in raise_intr cpu.stvec : 0x%lx\n",cpu.stvec);
  rtl_j(s,cpu.stvec);
}

void query_intr(DecodeExecState *s) {
}