#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();

enum return_status{
  QUIT=-1,
  SUCCESS,
  FAILURE,
  ILLEGAL_ARGS,
};

/* 读一行char*返回,并保存历史. We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

// 执行(unsigned)-1次
static int cmd_c(char *args) {
  cpu_exec(-1);
  return SUCCESS;
}

// 返回-1触发主循环退出条件
static int cmd_q(char *args) {
  return QUIT;
}

static int cmd_help(char *args);

// N步执行
static int cmd_si(char *args) {
  args = strtok(NULL," ");
  
  if(!args) {
    cpu_exec(1);
  }
  else {
    int steps = -1;
    if (sscanf(args,"%d",&steps)==-1 || steps <= 0) {return ILLEGAL_ARGS;}
    cpu_exec(steps);
  }
  
  return SUCCESS;
}

// 设置监视点
static int cmd_w(char *args) {
  if(args) {
    if(wp_set(args)) {
      printf("set %s\n", args);
      return SUCCESS;
    }
    else {
      printf("Set watchpoint failed\n");
      return FAILURE;
    }
  }
  return ILLEGAL_ARGS;
}

// 删除监视点
static int cmd_d(char *args) {
  args = strtok(NULL," ");
  if(args) {
    int n = -1;
    if(sscanf(args,"%d",&n) == -1 || n < 0) {return ILLEGAL_ARGS;}
    wp_delete(n);
    return SUCCESS;
  }

  return ILLEGAL_ARGS;
}

// 打印所有寄存器/WP
static int cmd_info(char *args) {
  args = strtok(NULL," ");
  if (args) {
    if(*args=='r') {
      isa_reg_display();
      return SUCCESS;
    }
    else if(*args=='w') {
      wp_display();
      return SUCCESS;
    }
  }
  return ILLEGAL_ARGS;
}

// 输入表达式返回值
static int cmd_p(char *args) {
  if(args) {
    int state = VALID_RET;
    /* 
     * expr错误处理（state返回值）:
     * VALID_RET       正常计算，有效返回值
     * DIV_BY_0        除零错误
     * MAKE_FAIL       非法表达式，正则表达式匹配失败
     * PARE_FAIL       非法表达式，括号失配
     * FIND_OP_FAIL    寻找主op出现错误
     * BAD_EXPR        非法表达式，无法计算的表达式
     * SSCANF_FAIL     sscanf发生错误
     * REG_FAIL        不存在的寄存器
     */ 
    word_t res = expr(args, &state);
    switch (state) {
    case VALID_RET:    printf("0x%lx\n",res);                                 return SUCCESS;
    case DIV_BY_0:     printf("Illegal expression, divided by 0\n");          return FAILURE;
    case MAKE_FAIL:    printf("Illegal expression, make_token failed\n");     return FAILURE;
    case PARE_FAIL:    printf("Illegal expression, invalid parentheses\n");   return FAILURE;
    case BAD_EXPR:     printf("Illegal expression, bad expression\n");        return FAILURE;
    case SSCANF_FAIL:  printf("Function call sscanf() failed\n");             return FAILURE;
    case REG_FAIL:     printf("Illegal expression, no such register\n");      return FAILURE;
    case FIND_OP_FAIL: printf("Illegal expression, cannot find (main) op\n"); return FAILURE;
    default:           break;
    }
  }
  return ILLEGAL_ARGS;
}


#include <memory/paddr.h>
// 求出表达式EXPR的值, 将结果作为起始内存地址
// 以十六进制形式输出连续的N个4字节
static int cmd_exam_addr(char *args) {
  char* arg1 = strtok(NULL," ");
  char* arg2 = strtok(NULL," ");

  // 取数字
  int n=-1;
  if (sscanf(arg1,"%d",&n) == -1 || n < 0) {return ILLEGAL_ARGS;}

  // 取表达式结果
  int state = VALID_RET;
  word_t addr = expr(arg2, &state);
  if(state != VALID_RET) {
    printf("Failure while calling expr()\n");
    return FAILURE;
  }

  // 打印地址
  printf("%u Byte start at Mem[0x%16lx]\n", n, addr);
  for(unsigned i=0; i<n; ++i) {
    printf("0x%016lx:  ", addr+4*i);
    for(int j=0; j<4; ++j)
      printf("%02x  ", (uint8_t)paddr_read(addr+4*i+j,1));
    putchar('\n');
  }

  return 0;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "\'si [N]\'\tExecute next N steps", cmd_si},
  { "info", "\'info r/w\'\tPrint values of all registers/watchpoints", cmd_info},
  { "x", "\'x N EXPR\'\tExam next 4N Bytes value from address EXRP", cmd_exam_addr},
  { "p", "\'p EXPR\'\tCompute value of EXPR", cmd_p},
  { "d", "\'d [N]\'\tDelete watchpoint N", cmd_d},
  { "w", "\'w EXPR\'\tset expression to stop program running when its value changes", cmd_w}
};

// number of cmds
#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s\t- %s\n", cmd_table[i].name, cmd_table[i].description);
    }
    return SUCCESS;
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    return ILLEGAL_ARGS;
  }
}

void ui_mainloop() {
  if (is_batch_mode()) {
    // printf("BATCH MODE, remember to use \"cmd_c\"\n");
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* 
     * treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    // 顺序查找匹配的命令，匹配调用handler，返回QUIT退出主循环
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        switch (cmd_table[i].handler(args)) {
        case QUIT:          return;
        case ILLEGAL_ARGS:  printf("Illegal arguments, try \"help\"\n");
        case FAILURE:
        case SUCCESS:
        default:            break;
        }
        break;
      }
    }

    // 没找到说明输入有问题
    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
