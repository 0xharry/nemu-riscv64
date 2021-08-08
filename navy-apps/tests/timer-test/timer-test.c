#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval time;
    int sec = 1;
    int j = 0;
    while (1) {
        j++;
        if (j == 100000) {
            _syscall_(19, &time, 0, 0);
            int pre_usec = time.tv_usec;
            printf("sec : %ld, usec : %ld\n",time.tv_sec, time.tv_usec);
            j = 0;
        }
    }
    return 0;
}