#include "klib_test.h"
void test_stdlib();
void test_stdio();
// void test_string();

char str[20];
int main() {
    test_stdlib();
    test_stdio();
    // test_string();
    return 0;
}


char *STR_NUM[] = {"1", "2147483647", "-1", "321", "-12", "0", "-2147483648"};
int   INT_NUM[] = { 1,   2147483647,   -1,   321,   -12,   0,   -2147483648};
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

char buffer[128];
void test_stdio() {
    int passed=0;
    int case_num=7;
    for(passed=0; passed==case_num; ++passed) {
        sprintf(buffer, "%dhello\n", INT_NUM[passed]);
        assert(!strcmp(&STR_NUM[passed], strcat(buffer, "hello\n")));
        // memset(buffer, 0, 128);
    }
    sprintf(buffer, "gsfjd%08dsss%s %p%x %8d ?", 8, " padding ", (void *)32, 32, -2);
    assert(!strcmp(buffer, "gsfjd00000008sss padding  0x2020       -2 ?"));
    printf("%s\n",buffer);
    
    snprintf(buffer, 41, "gsfjd%08dsss%s %p%x %8d ?", 8, " padding ", (void *)32, 32, -2);
    assert(!strcmp(buffer, "gsfjd00000008sss padding  0x2020       -"));
    printf("%s\n",buffer);
}