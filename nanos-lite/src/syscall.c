#include <common.h>
#include "syscall.h"
#include "/home/harry/ics2020/navy-apps/libs/libos/src/syscall.h"
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = _a7;
  a[1] = _a0;
  a[2] = _a1;
  a[3] = _a2;
  
  // for(int i=0; i<31; ++i) {
  //   printf("x%d : %p\n", i+1, (void*)c->gpr[i]);
  // }

  switch (a[0]) {
    case SYS_exit :  call_(exit);  break;
    case SYS_yield:  call_(yield); break;
    // case SYS_open :
    // case SYS_read :
    case SYS_write:  call_(write); break;
    // case SYS_close:
    // case SYS_lseek:
    case SYS_brk  :  call_(brk);   break;
    case SYS_execve:
    case SYS_gettimeofday:
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
