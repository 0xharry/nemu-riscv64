#include <stdio.h>
#include <sys/time.h>

int main() {
  struct timeval t;
  gettimeofday(&t,NULL);
  int sec = t.tv_sec;
  int half_sec_glass=((t.tv_usec / 500000) == 1);
  while(!gettimeofday(&t,NULL)) {
    half_sec_glass ^= ((t.tv_usec / 500000) == 1);
    if((long)sec <= t.tv_sec) {
      printf("一句话 ");
      sec++;
    }
  }
  return 0;
}