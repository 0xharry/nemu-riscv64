#include <stdio.h>
#include <sys/time.h>
#include <assert.h>
#include <NDL.h>


// int main() {
//     struct timeval time;
//     int sec = 1;
//     int j = 0;
//     while (1) {
//         j++;
//         if (j == 100000) {
//             gettimeofday(&time,NULL);
//             int pre_usec = time.tv_usec;
//             printf("sec : %ld, usec : %ld\n",time.tv_sec, time.tv_usec);
//             j = 0;
//         }
//     }
//     return 0;
// }

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