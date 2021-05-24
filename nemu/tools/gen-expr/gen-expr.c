#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 1024] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n\
#include \"/home/harry/ics2020/nemu/tools/gen-expr/expr.h\"\n\
int main() {  \
  init_regex(); \
  static char exp[65536] = \"%s\"; \
  bool flag=true; \
  unsigned result = expr(exp,&flag);  \
  if(flag) printf(\"%%u\", result);  \
  return 0;  \
}";

static short count=0;

void gen_num()
{
  buf[count++] = '0'+rand()%10;
}

// 25%几率生成空格
void gen_space()
{
  while (!rand()%4)
    buf[count++] = ' ';
}

void gen(char c)
{
  gen_space();
  buf[count++] = c;
}

void gen_rand_op()
{
  gen_space();
  switch (rand()%4) {
    case 0: buf[count++] = '+'; break;
    case 1: buf[count++] = '-'; break;
    case 2: buf[count++] = '*'; break;
    default: buf[count++] = '/'; break;
  }
}

static inline void gen_rand_expr() {
  switch (rand()%3) {
    case 0: gen_num(); break;
    case 1: gen('('); gen_rand_expr(); gen(')'); break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  // 生成的表达式个数
  if (argc > 10) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    count = 0;
    memset(buf,0,sizeof(buf));
    gen_rand_expr();

    // 生成的表达式转成c代码放到code_buf
    sprintf(code_buf, code_format, buf);

    // code_buf中的代码写入code.c文件
    FILE *fp = fopen("/home/harry/ics2020/nemu/temp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    // code.c gcc生成可执行文件
    int ret = system("make tmp");
    if (ret != 0) continue;

    // 打开可执行文件
    fp = popen("/home/harry/ics2020/nemu/temp/.expr", "r");
    assert(fp != NULL);

    // 将表达式计算结果保存，连同表达式一起输出到屏幕
    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
