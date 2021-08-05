#include "watchpoint.h"
#include "expr.h"
#include<stdlib.h>

extern uint64_t expr(char *, bool *);

#define NR_WP 32


WP* new_wp();
void free_wp(WP *wp);

//32个wp池
static WP wp_pool[NR_WP] = {};
//head是使用中的，free用于空闲监视点
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
    //wp_pool[i].used = false;
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

bool is_empty(WP * list){
  return list==NULL;
}

WP* new_wp(){
  //从全局空闲链表中分配
  if(is_empty(free_)){
    printf("no memory in free_ list\n");
    return NULL;
  }
  WP * dummy = malloc(sizeof(WP));
  dummy = free_;
  free_=free_->next;
  dummy->next=head;
  head = dummy;
  return head;
}

//将wp加入到free,并从head中删除
void free_wp(WP *wp){
  if(is_empty(head)){
    printf("no memory in head list\n");
    return ;
  }
  if(wp==head){
    WP * p = head;
    head = head->next;
    p->next = free_;
    free_ = p;
  }

  WP * p = head;
  while(p->next && p->next!=wp) p = p->next;
  p->next = p->next->next;
  wp->next = free_;
  free_ = wp;
}

//根据名字设置监视点，加入到head中
void set_wp(char *name){
  WP * wp_entry =  (WP*)malloc(sizeof(WP));
  wp_entry = free_; 
  free_ = free_->next;
  wp_entry->next= head;
  head = wp_entry;
  strcpy(wp_entry->str,name);
  bool flag;
  uint64_t val = expr(name,&flag);
  printf("watchpoint %d initial val : %lu\n",wp_entry->NO,val);
  wp_entry->oldVal = val;
}

//根据名字删除监视点，从head中移除，加入到free_
void del_wp(int no){
  bool flag = true;
  WP * p = head;
  WP * pprev = NULL;
  while(p){
    if(p->NO==no){
      flag = true;
      //头节点需要特殊考虑
      if(p==head){
        head = p->next;
      }
      else{
        pprev->next = p->next;
      }
      p->next = free_;
      free_ = p;
      printf("NO%d has been removed!\n",no);
      print_watchpoint();
    }
    pprev = p;
    p = p->next;
  }
  if(flag==false) printf("No such NO in watchpoint\n");
}


//检查单个的监视点的值是否发生了变化
//相同true,不同false
bool check_wp(WP *wp){
  bool flag = true;
  uint64_t val = expr(wp->str,&flag);
  wp->oldVal = wp->newVal;
  wp->newVal = val;
  return wp->oldVal==wp->newVal;
}

//检查所有的监视点是否有值发生改变
bool check_all_wp(){
  WP * p=head;
  bool flag = true;
  //每次运行时会更新所有wp
  while(p){
    if(check_wp(p)==false){
      flag = false;
      printf("NO%d \n",p->NO);
      printf("%lu\n",p->oldVal);
      printf("%lu\n",p->newVal);
    }
    p = p->next;
  }
  return flag;
}

//打印正在使用中的watchpoint
void print_watchpoint(){
  if(head==NULL){
    printf("No watch point here\n");
    return;
  }

  WP *p=head;
  printf("NO      name    oldVal  newVal\n");
  while(p){
    printf("%-8d%-8s%-8lu%-8lu\n",p->NO,p->str,p->oldVal,p->newVal);
    p = p->next;
  }
  puts("");
}