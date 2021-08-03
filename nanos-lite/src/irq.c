#include <common.h>
#include "/home/harry/ics2020/abstract-machine/am/include/am.h"

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    // case EVENT_NULL: break;
    // case EVENT_YIELD: break;
    case EVENT_SYSCALL: 
      Log("user_handler(do_event()): case EVENT_SYSCALL");
      break;
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
