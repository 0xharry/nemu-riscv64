#include <common.h>
#include "syscall.h"
#include "/home/harry/ics2020/navy-apps/libs/libos/src/syscall.h"
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_yield: yield();
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
