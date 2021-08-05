#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 1280] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"#include<signal.h>\n"
"#include<sys/types.h>\n"
"void f(){"
"  printf(\"can not divide by zero\");"
"}\n"
"int main() { "
"  unsigned result = (unsigned)%s; "
"  signal(SIGFPE,f);"
"  printf(\"%%u\", result); "
"  return 0; "
"}";

int idx=0;

#define SPACE do{if(rand()%2==0) gen(' ');}while(0)

static uint32_t choose(int n){
  return rand()%n;
}

void gen(char c){
  buf[idx++] = c;
}

//生成最多两位数
void gen_num(){
  int num = choose(100);
  if(num>=10){
    buf[idx++] = num/10 + '0';
    buf[idx++] = num%10 + '0';
  }
  else{
    buf[idx++]=num+'0';
  }
}

void gen_rand_op(){
  switch(choose(3)){
    case 0: gen('+'); break;
    case 1: gen('-'); break;
    case 2: gen('*'); break;
    case 3: gen('/'); break;
    default:break;
  }
}

static inline void gen_rand_expr() {
  switch (choose(3)) {
    case 0: gen_num(); break;
    case 1: gen('('); SPACE; gen_rand_expr(); SPACE; gen(')'); break;
    default: if(idx>65536)break; else{ gen_rand_expr(); gen_rand_op(); gen_rand_expr();} break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 100;
  if (argc > 1) {
    //注意sscanf的运用
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    //每次循环都要将buf设置为初始状态
    idx=0;
    memset(buf,0,strlen(buf));
    gen_rand_expr();
    buf[idx] = '\0';
    //buf按照code_format格式写入code_buf
    //code_buf就是一个c代码
    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    //code_buf写入.c文件
    fputs(code_buf, fp);
    fclose(fp);

    //执行shell命令
    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;
    // 调用fork()产生子进程，然后从子进程中调用/bin/sh -c 来执行参数command 的指令。
    //因为利用system函数调用shell命令，只能获取到shell命令的返回值，而不能获取shell命令的输出结果
    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    //将输出结果写入到result
    if(fscanf(fp, "%d", &result));
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
