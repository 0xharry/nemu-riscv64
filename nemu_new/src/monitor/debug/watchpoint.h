#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__
#include <common.h>

typedef struct watchpoint {
  //序号
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  uint64_t oldVal;
  uint64_t newVal;
  // bool used; 已经有了两个全局链表，无需使用标志位
  char str[128];
} WP;

//根据名字设置监视点，加入到head中
void set_wp(char *);
//根据名字删除监视点，从head中移除，加入到free_
void del_wp(int);
//打印所有wp的 NO str oldVal newVal
void print_watchpoint();
//检查单个的监视点的值是否发生了变化
bool check_wp(WP *);
//检查所有的监视点是否有值发生改变
bool check_all_wp();
//判断WP*链表是否为空
bool is_empty(WP*);

#endif
