// CTE(Context Extension) - 上下文扩展, 为程序提供上下文管理的能力
#include <am.h>
#include <nemu.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    switch (c->cause) {
      // case EVENT_NULL: break;
      // case EVENT_YIELD: break;
      case EVENT_SYSCALL: ev.event = EVENT_SYSCALL; break;
      // case EVENT_PAGEFAULT: break;
      // case EVENT_IRQ_TIMER: break;
      // case EVENT_IRQ_IODEV: break;
      default: ev.event = c->cause; break;
    }
#ifdef DEBUG
    printf("&c=%p\n&c->gpr[1]=%p\nc->cause=%p\n", c, &c->gpr[1], &c->cause);
    printf("Context:cause=%d\tepc=%p\tstatus=%d\n",c->cause,c->epc,c->status);
    for(int i=0;i<31;++i){
      printf("%p\n",c->gpr[i]);
    }
#endif
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  return c;
}

extern void __am_asm_trap(void);

// 进行CTE相关的初始化操作, CTE将会把事件和相关的上下文作为参数,
// 接受一个来自操作系统的事件处理回调函数的指针
// (当发生事件时, CTE将会把事件和相关的上下文作为参数, 调用这个回调函数, 交由操作系统进行后续处理.)
bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  // 设置异常入口地址: 将异常入口地址设置到stvec寄存器
  asm volatile("csrw stvec, %0" : : "r"(__am_asm_trap));

  // register event handler 
  // 注册一个事件处理回调函数, 由Nanos-lite提供
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
  asm volatile("li a7, -1; ecall");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
