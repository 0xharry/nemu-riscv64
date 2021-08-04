#include <common.h>
#include "syscall.h"
#include "/home/harry/ics2020/navy-apps/libs/libos/src/syscall.h"
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // for(int i=0; i<30; ++i) {
  //   printf("%d : %p\n", i, (void*)c->gpr[i]);
  // }

  switch (a[0]) {
    case SYS_yield: yield(); break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
