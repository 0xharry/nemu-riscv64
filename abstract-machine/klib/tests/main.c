#include "klib_test.h"
void test_stdlib();
// void test_stdio();
// void test_string();

int main() {
    test_stdlib();
    // test_stdio();
    // test_string();
    return 0;
}


char *STR_NUM[] = {"1", "-1", "321", "-12", "0", "2147483647", "-2147483648"};
int   INT_NUM[] = { 1,   -1,   321,   -12,   0,   2147483647,   -2147483648};
void test_stdlib() {
    int i, loop_size;
    // atoi
    loop_size = 7;
    for(i=0; i<loop_size; ++i) {
        assert(atoi(STR_NUM[i]) == INT_NUM[i]);
    }
}

// char buffer[128];
// char **STR_STR = {"-2+DQ8[];232(*$s@_D!53+15-8R  FA"};
// void test_string() {
//     int i, loop_size;
//     // memset
//     loop_size = 2*sizeof(int);
//     char temp_char = (char)-1;
//     memset(buffer,-1,loop_size);
// }

// void test_stdio() {
//     int passed=1;
//     int case_num=0;
//     char *info_buf = "passed all %d cases";


//     if(passed)
//         printf("%s", case_num);
// }