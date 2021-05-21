#include "watchpoint.h"
#include "expr.h"

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
    return NULL; // personally, no need to call assert(0);
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
    printf("No action, NULL wp / no active wp");
    return;
  }

  // edge case: wp == head
  if(wp == head)
  {
    memset(wp->expr, '\0', sizeof(WP)*NR_WP);
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
      memset(wp->expr, '\0', sizeof(WP)*NR_WP);
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

bool wp_set(char* expr)
{
  WP* wp = wp_new();
  if(!wp)
  {
    printf("wp_new failed\n");
    return false;
  }
  if(!strcpy(wp->expr, expr))
  {
    return false;
  }
  return true;
}

void wp_delete(int n)
{
  // delete all
  if(n == -1)
  {
    printf("Delete all breakpoints? (y or n) ");
    char ans = 'n';
    scanf("%c", &ans);
    if(ans=='y'||ans=='Y')
    {
      while(!head)
      {
        wp_free(head);
      }
    }
    else
      printf("No action, continue\n");
  }

  if(n<-1 || n>NR_WP-1)
    printf("watchpoint %d do not exist\n", n);

  wp_free(wp_pool+n);
}

bool wp_check()
{
  bool all_clear = true;
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
  return all_clear;
}

void wp_display()
{
  if(!head)
  {
    printf("\tNum\tWhat");
    for(WP* p = head; p != NULL; p=p->next)
    {
      printf("\t%d\t%s\n", p->NO, p->expr);
    }
  }
  else
  {
    printf("No watchpoints.");
  }
}