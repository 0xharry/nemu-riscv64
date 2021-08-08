#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>

#ifdef __ISA_NATIVE__
#error can not support ISA=native
#endif

#define SYS_gettimeofday 19
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

int main() {
  struct timeval t;
  _syscall_(SYS_gettimeofday, 0, &t, 0);
  int half_sec_glass=(t.tv_usec / 500000 < 1);
  while(1) {
    _syscall_(SYS_gettimeofday, 0, &t, 0);
    half_sec_glass ^= (t.tv_usec / 500000 < 1);
    while(half_sec_glass) {
      printf("一句话\n");
    }
  }
  return 0;
}
