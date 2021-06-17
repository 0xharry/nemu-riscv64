#include <am.h>
#include <nemu.h>
#include "/home/harry/ics2020/abstract-machine/klib/include/klib.h"

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
    kbd->keydown = (inl(KBD_ADDR) & KEYDOWN_MASK ? true : false);
    kbd->keycode = inl(KBD_ADDR) & ~KEYDOWN_MASK;
  if(kbd->keycode != 0) {
printf("input.c: %s\t(%u)\n", kbd->keydown ? "down" : "up", kbd->keycode);
  }
}
