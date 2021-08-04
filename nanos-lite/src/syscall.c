#include <common.h>
#include "syscall.h"
#include "/home/harry/ics2020/navy-apps/libs/libos/src/syscall.h"
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  // for(int i=0; i<30; ++i) {
  //   printf("%d : %p\n", i, (void*)c->gpr[i]);
  // }

  switch (a[0]) {
    case SYS_exit :  call_exit(a[1]);  break;
    case SYS_yield:  yield();          break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
