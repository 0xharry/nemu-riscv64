#include <am.h>
#include <nemu.h>

extern char _heap_start;
int main(const char *args);

//堆区的起始和结尾
Area heap = RANGE(&_heap_start, PMEM_END);
#ifndef MAINARGS
#define MAINARGS ""
#endif
static const char mainargs[] = MAINARGS;

//输出一个字符
void putch(char ch) {
  outb(SERIAL_PORT, ch);
}

//结束程序的运行
void halt(int code) {
  nemu_trap(code);

  // should not reach here
  while (1);
}

//trm相关的初始化
void _trm_init() {
  int ret = main(mainargs);
  halt(ret);
}
