#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include <common.h>

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  // bool status;
  char expr[128];
  word_t pre_state_val;
} WP;

WP* wp_new();
bool wp_set(char* expr);
void wp_delete(int n);
bool wp_check();
void wp_display();

#endif
