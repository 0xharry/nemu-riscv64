#include <common.h>
#include "syscall.h"


extern void do_syscall(Context *c);

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case EVENT_YIELD:     break;
    case EVENT_SYSCALL:   do_syscall(c);   break;
    // case EVENT_PAGEFAULT: Log("EVENT_PAGEFAULT")  ;break;
    case EVENT_ERROR:     Log("Event error")      ;break;
    // case EVENT_IRQ_TIMER: Log("EVENT_IRQ_TIMER")  ;break;
    // case EVENT_IRQ_IODEV: Log("EVENT_IRQ_IODEV")  ;break;
    default: panic("Unhandled event ID = %d", e.event);
  }

  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
