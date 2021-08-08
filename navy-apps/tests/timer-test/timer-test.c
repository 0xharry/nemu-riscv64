#include <stdio.h>
#include <sys/time.h>
#include <NDL.h>

int main() {
//   NDL_Init(0);
//   unsigned ms=NDL_GetTicks();
//   while(1) {
//       while(NDL_GetTicks() < ms);
//       ms+=10;
//       printf("一句话");
//   }
//   return 0;
NDL_Init(0);
    struct timeval time;
    int halfsec = 1;
    int j = 0;
    while(1) {
        j++;
        if(j==100000) {
            //因为不精确，所以不能整除来求
            int cur = NDL_GetTicks()/500000;
            if(cur >= halfsec) {
                printf("0.5s pass\n");
                halfsec++;
            }
        }
    }
}