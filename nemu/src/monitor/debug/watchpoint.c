#include "watchpoint.h"
#include "expr.h"
#include <stdlib.h>
#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = &wp_pool[i + 1];
  }
  wp_pool[NR_WP - 1].next = NULL;

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* wp_new()
{
  if(!free_)
  {
    printf("Watch point pool is dried. ");
    return NULL;
  }
  WP* curr = free_;
  free_ = free_->next;
  curr->next = head;
  head = curr;
  return head;
}

void wp_free(WP *wp)
{
  // edge case: free NULL
  if(!head || !wp)
  {
    printf("No action, NULL wp / no active wp\n");
    return;
  }

  // edge case: wp == head
  if(wp == head)
  {
    free(wp->expr);
    head = head->next;
    wp->next = free_;
    free_ = wp;
    printf("free watchpoint %d\n", wp->NO);
    return;
  }
  
  for(WP* curr = head; curr->next != NULL; curr = curr->next)
  {
    if(curr->next == wp)
    {
      free(wp->expr);
      curr->next = wp->next;
      wp->next = free_;
      free_ = wp;
      printf("free watchpoint %d\n", wp->NO);
      return;
    }
  }
  printf("No action, watchpoint not found");
  assert(0);
}
/* ~~~ */
static inline void helper(char* dst,const char* src) {
  if(dst == NULL || src == NULL) return;
  char* p_dst = dst;
  const char* p_src = src;
  while((bool)(*p_dst++ = *p_src++));
}
/* ~~~ */
bool wp_set(char* input_expr)
{
  int state;
  word_t expr_val = expr(input_expr, &state);
  if(state != VALID_RET)
    return false;

  WP* wp = wp_new();
  if(!wp)
  {
    printf("wp_new failed\n");
    return false;
  }
  char *wp_expr = (char*)malloc(sizeof(input_expr));
  if(!wp_expr)
  {
    wp_free(wp);
    printf("malloc for expression failed\n");
    return false;
  }
  helper(wp_expr, input_expr);
  wp->expr = wp_expr;
  wp->pre_state_val = expr_val;
  return true;
}

void wp_delete(int n)
{
  // delete all
  if(n == -1)
  {
    printf("Delete all breakpoints? (y or n) ");
    char ans = 'n';
    if(scanf("%c", &ans)==EOF) assert(0);
    if(ans=='y'||ans=='Y')
    {
      while(!head)
      {
        wp_free(head);
      }
      return;
    }
    else
    {
      printf("No action, continue\n");
      return;
    }
  }

  if(n<-1 || n>NR_WP-1)
  {
    printf("watchpoint %d do not exist\n", n);
    return;
  }

  wp_free(wp_pool+n);
}

bool wp_check()
{
  int all_clear = VALID_RET;
  word_t curr_val;
  for(WP* curr=head; curr!=NULL; curr=curr->next)
  {
    curr_val = expr(curr->expr, &all_clear);
    if(curr->pre_state_val != curr_val)
    {
      printf("watchpoint %d: %s\nold value = 0x%lx\nnew value = 0x%lx\n"\
      , curr->NO, curr->expr, curr->pre_state_val, curr_val);
      curr->pre_state_val = curr_val;
      all_clear = false;
    }
  }
  return (bool)all_clear;
}

void wp_display()
{
  if(head)
  {
    printf("Num\tWhat\n");
    for(WP* p = head; p != NULL; p=p->next)
    {
      printf("%d\t%s\n", p->NO, p->expr);
    }
  }
  else
  {
    printf("No watchpoints.\n");
  }
}