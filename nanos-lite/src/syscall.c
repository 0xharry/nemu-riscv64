#include <common.h>
#include "syscall.h"
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"
#define _a7  c->GPR1
#define _a0  c->GPR2
#define _a1  c->GPR3
#define _a2  c->GPR4
#define _ret c->GPRx
extern int fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
// extern int fs_close(int fd);

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = _a7;
  a[1] = _a0;
  a[2] = _a1;
  a[3] = _a2;

  switch (a[0]) {
    case SYS_exit :  halt(_a0);                       break;
    case SYS_yield:  yield();                         break;
    case SYS_open :  _ret = fs_open ((char*)_a0, _a1, _a2);  break;
    case SYS_read :  _ret = fs_read (_a0, (char*)_a1, _a2);  break;
    case SYS_write:  _ret = fs_write(_a0, (char*)_a1, _a2);  break;
    case SYS_close:  _ret = 0;                        break;
    case SYS_lseek:  _ret = fs_lseek(_a0, _a1, _a2);         break;
    case SYS_brk  :  _ret = 0;                        break;
    // case SYS_execve:
    // case SYS_gettimeofday:
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
