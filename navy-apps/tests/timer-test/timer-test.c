#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  // struct timeval t;
  // _syscall_(SYS_gettimeofday, 0, &t, 0);
  // int sec = t.tv_sec;
  // int half_sec_glass=((t.tv_usec / 500000) == 1);
  // while(!_syscall_(SYS_gettimeofday, 0, &t, 0)) {
  //   half_sec_glass ^= ((t.tv_usec / 500000) == 1);
  //   if((long)sec <= t.tv_sec) {
  //     printf("一句话, sec=%d, half_sec_glass=%d\n", sec, t.tv_usec);
  //     sec++;
  //   }
  // }
  struct timeval t;
  int sec = 1;
  _syscall_(SYS_gettimeofday, 0, &t, 0);
  int base = t.tv_sec;
  while (1) {
    _syscall_(SYS_gettimeofday, 0, &t, 0);
    while(t.tv_sec-base < sec);
    printf("一句话\n");
    sec++;
  }
  return 0;
}
