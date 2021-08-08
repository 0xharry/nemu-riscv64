#include <common.h>
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"
#define sys_call_num c->gpr[16]
extern void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    // case EVENT_NULL: break;
    case EVENT_YIELD:
      printf("[EVENT]: EVENT_YIELD, do nothing\n"); 
      break;
    case EVENT_SYSCALL: 
      printf("[EVENT]: EVENT_SYSCALL, num=%u\n", sys_call_num); 
      do_syscall(c); break;
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
