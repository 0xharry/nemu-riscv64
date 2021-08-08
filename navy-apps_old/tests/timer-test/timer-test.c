#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <NDL.h>

int main() {
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