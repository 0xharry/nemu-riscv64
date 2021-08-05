// CTE(Context Extension) - 上下文扩展, 为程序提供上下文管理的能力
#include <am.h>
#include <nemu.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    c->epc += 4;
    Event ev = {0};
    // if(c->gpr[16]<=19 && c->gpr[16]>=0) ev.event = EVENT_SYSCALL;
    // else if(c->gpr[16]==-1)             ev.event = EVENT_YIELD;
    // else                                ev.event = EVENT_ERROR;
    switch (c->cause) {
      case  9: ev.event = EVENT_SYSCALL; break; // syscall number $a7, arguments $a0~5
      default: ev.event = EVENT_ERROR;   break;
    }
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

// 进行CTE相关的初始化操作, CTE将会把事件和相关的上下文作为参数,
// 接受一个来自操作系统的事件处理回调函数的指针
bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler 
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, 1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
