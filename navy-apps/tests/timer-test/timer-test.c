#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  struct timeval time;
  int sec = 1;
  int j = 0;
  while (1) {
      j++;
      if (j == 100000) {
          gettimeofday(&time,NULL);
          int pre_usec = time.tv_usec;
          printf("sec : %ld, usec : %ld\n",time.tv_sec, time.tv_usec);
          j = 0;
      }
  }
  return 0;
  // struct timeval t;
  // _syscall_(SYS_gettimeofday, 0, &t, 0);
  // double sec = t.tv_sec;
  // int half_sec_glass=((t.tv_usec / 500000) == 1);
  // while(!_syscall_(SYS_gettimeofday, 0, &t, 0)) {
  //   half_sec_glass ^= ((t.tv_usec / 500000) == 1);
  //   if((long)sec <= t.tv_sec && half_sec_glass) {
  //     printf("一句话, sec=%d, half_sec_glass=%d\n", (long)sec, t.tv_usec);
  //     sec += 0.5;
  //   }
  // }
  // return 0;
}
