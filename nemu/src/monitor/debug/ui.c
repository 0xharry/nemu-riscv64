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
  // TODO: consider if args are illegal
  args = strtok(NULL," ");
  if(!args)
    cpu_exec(1);
  else
    cpu_exec(*args-'0');
  return SUCCESS;
}

// 设置监视点
static int cmd_w(char *args)
{
  if(args)
  {
    if(wp_set(args))
    {
      printf("set %s\n", args);
      return SUCCESS;
    }
    else
      return FAILURE;
  }
  return ILLEGAL_ARGS;
}

// 删除监视点
static int cmd_d(char *args)
{
  args = strtok(NULL," ");
  if(args)
  {
    int n;
    if(sscanf(args, "%d", &n) == -1)
      Assert(0, "sscanf fault");
    wp_delete(n);
    return SUCCESS;
  }
  return ILLEGAL_ARGS;
}

// 打印所有寄存器/WP
static int cmd_info(char *args) {
  args = strtok(NULL," ");
  if (args)
  {
    if(*args=='r')
    {
      isa_reg_display();
      return SUCCESS;
    }
    else if(*args=='w')
    {
      wp_display();
      return SUCCESS;
    }
  }
  return FAILURE;
}

#include <memory/paddr.h>
// 检查指定地址附近的值
static int cmd_exam_addr(char *args) {
  // TODO: illegal args and unreadable code need refinement
  unsigned n = *strtok(NULL," ") - '0';
  word_t expr = strtoul(strtok(NULL," "),NULL,16);
  printf("%u Byte start at Mem[0x%16lx]\n", n, expr);
  for(unsigned i=0; i<n; ++i)
  {
    printf("0x%016lx:  ", expr+4*i);
    for(int j=0; j<4; ++j)
      printf("%02x  ", (uint8_t)paddr_read(expr+4*i+j,1));
    putchar('\n');
  }
  return 0;
}

// 输入表达式返回值
static int cmd_p(char *args) {
  if(args)
  {
    bool flag = true;
    word_t res = expr(args, &flag);
    if(flag)
      printf("0x%lx\n",res);
    return SUCCESS;
  }
  return ILLEGAL_ARGS;
}

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "\t\t\'si [N]\'Execute next N steps", cmd_si},
  { "info", "\t\t\'info r/w\'Print values of all registers/watchpoints", cmd_info},
  { "x", "\t\t\'x N EXPR\'Exam next 4N Bytes value from address EXRP", cmd_exam_addr},
  { "p", "\t\t\'p EXPR\'Compute value of EXPR", cmd_p},
  { "d", "\t\t\'d [N]\'Delete watchpoint N", cmd_d},
  { "w", "\t\t\'w EXPR\'set expression to stop program running when its value changes", cmd_w},
  /* TODO: Add more commands */

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
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* ？？？
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
        switch (cmd_table[i].handler(args))
        {
        case QUIT:
          return;

        case ILLEGAL_ARGS:
          printf("Illegal arguments, try \"help\"");
        case FAILURE:
        case SUCCESS:
        default:
          break;
        }
        break;
      }
    }

    // 没找到那就有问题
    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
