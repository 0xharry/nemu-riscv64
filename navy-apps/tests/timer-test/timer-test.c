#include <stdio.h>
#include <sys/time.h>
#include <NDL.h>

int main() {
  NDL_Init(0);
  unsigned ms=NDL_GetTicks();
  while(1) {
      while(NDL_GetTicks() < ms);
      ms+=1000;
      printf("一句话");
  }
  return 0;
}