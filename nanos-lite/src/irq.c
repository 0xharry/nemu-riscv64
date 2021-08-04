#include <common.h>
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"
extern void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    // case EVENT_NULL: break;
    case EVENT_YIELD:
      printf("[EVENT]: EVENT_YIELD\n"); break;
    case EVENT_SYSCALL: 
      printf("[EVENT]: EVENT_SYSCALL\n"); do_syscall(c); break;
    // case EVENT_PAGEFAULT: break;
    // case EVENT_ERROR: break;
    // case EVENT_IRQ_TIMER: break;
    // case EVENT_IRQ_IODEV: break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
