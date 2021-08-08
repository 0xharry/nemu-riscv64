#include <common.h>
#include "syscall.h"
#include "fs.h"
#include<sys/time.h>


extern void yield();


extern int    fs_open(const char *pathname, int flags, int mode);
extern size_t fs_read(int fd, void *buf, size_t len);
extern size_t fs_write(int fd, const void *buf, size_t len);
extern size_t fs_lseek(int fd, size_t offset, int whence);
extern int    fs_close(int fd);
size_t _write(int fd, void *buf, size_t count);
int sys_gettimeofday(struct timeval *tv, struct timezone *tz);


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;  //第一个参数是系统调用号
  a[1] = c->GPR2;  // a0
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  switch (a[0]) {
    case SYS_exit:  halt(a[1]); break;
    case SYS_yield: c->GPRx = 0;  yield(); break;
    case SYS_open:  c->GPRx = fs_open((char*)a[1],a[2],a[3]);  break;
    case SYS_read:  c->GPRx = fs_read(a[1],(void *)a[2],a[3]); break;
    case SYS_write: c->GPRx = fs_write(a[1],(void *)a[2],a[3]);break;
    case SYS_close: c->GPRx = fs_close(a[1]);                  break;   
    case SYS_lseek: c->GPRx = fs_lseek(a[1],a[2],a[3]);        break;
    case SYS_brk  : c->GPRx = 0;                               break;
    case SYS_gettimeofday : c->GPRx = sys_gettimeofday((struct timeval *)a[1],(struct timezone *)a[2]);break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}



size_t _write(int fd, void *buf, size_t count){
  //size_t ret = ramdisk_write(buf,0,count);
  size_t ret = count;
  if(fd == 1 || fd == 2) {
    int i;

    for(i=0;i<count;i++) {
      putch(*(char *)(buf+i));
    }
    return ret;
  }
  return -1;
}

