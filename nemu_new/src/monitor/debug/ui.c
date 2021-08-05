#include <isa.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "isa/riscv64.h"
#include "memory/vaddr.h"
#include "memory/paddr.h"

#define CLOSE "\001\033[0m\002"                 // 关闭所有属性
#define BLOD  "\001\033[1m\002"                 // 强调、加粗、高亮
#define BEGIN(x,y) "\001\033["#x";"#y"m\002"    // x: 背景，y: 前景

void cpu_exec(uint64_t);
int is_batch_mode();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  //TODO:已经置空了为什么还需要判断？
  if (line_read) {
    free(line_read);
    line_read = NULL;
  }
  //readline returns the text of the line read.
  line_read = readline(BEGIN(49, 34)"Myshell $ "CLOSE);

  if (line_read && *line_read) {
    //自动补全功能
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  //INT_MAX
  cpu_exec(-1);
  return 0;
}

//输入q退出
static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

//impletion helper functions
static int cmd_s(char *args){
  char *arg = strtok(NULL, " ");
  if(!arg){
    cpu_exec(1);
  }
  else{
    int step = atoi(arg);
    cpu_exec(step);
  }
  return 0;
}


static int cmd_i(char *args){
  char *arg = strtok(NULL, " ");
  if(arg[0]=='r')
    isa_reg_display();
  //打印使用中的监视点
  if(arg[0]=='w'){
    print_watchpoint();
  }
  return 0;
}


static int cmd_x(char *args){
  char *arg1 = strtok(NULL, " ");
  //没有额外参数，err
  if(!arg1) return -1;
  char *arg2 = strtok(NULL, " ");
  int num = atoi(arg1);
  word_t addr=0;
  sscanf(arg2,"0x%lx",&addr);
  int len=0;
  while(num--){
      len++;
      word_t vaddr = vaddr_read(addr,1);
      printf("%02lx ",vaddr);
      addr+=1;
      if(len%4==0) puts("");
  }
  puts("");
  return 0;
}

//设置监视点
static int cmd_w(char * args){
  char *name = strtok(NULL, " ");
  set_wp(name);
  return 0;
}

static int cmd_d(char * args){
  char *num = strtok(NULL, " ");
  del_wp(atoi(num));
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
  { "s", "exec one instruction per step",cmd_s},
  { "i", "print the state of registers",cmd_i},
  { "x", "scanf memory",cmd_x},
  { "w", "set watch point",cmd_w},
  { "d", "delete watch point",cmd_d},
  /* TODO: Add more commands */
};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
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

    /* treat the remaining string as the arguments,
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
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
