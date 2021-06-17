#include <am.h>
#include <nemu.h>
#include "/home/harry/ics2020/abstract-machine/klib/include/klib.h"

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  int kc = inl(KBD_ADDR) & ~KEYDOWN_MASK;
  if(kc) {
    kbd->keydown = (inl(KBD_ADDR) & KEYDOWN_MASK ? true : false);
    kbd->keycode = kc;
printf("%s\t(%u)\n", kbd->keydown ? "down" : "up", kc);
  }
}
