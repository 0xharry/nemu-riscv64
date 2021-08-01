// CTE(Context Extension) - 上下文扩展, 为程序提供上下文管理的能力
#include <am.h>
#include <nemu.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
///////////////////////////////////////
    for(int i=0; i<32; ++i)
      printf("%x ", c->gpr[i]);
    printf("\n%x,%x,%x\n", c->cause, c->status, c->epc);
//////////////////////////////////////
    Event ev = {0};
    switch (c->cause) {
      default: ev.event = EVENT_ERROR; break;
    }

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
