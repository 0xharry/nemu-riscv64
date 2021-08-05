#include <am.h>
#include <nemu.h>
#include "/home/harry/ics2020/abstract-machine/klib/include/klib.h"

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t am_scancode = inl(KBD_ADDR);
  kbd->keydown = (am_scancode & KEYDOWN_MASK ? true : false);
  kbd->keycode = am_scancode & ~KEYDOWN_MASK;
//   if(kbd->keycode != 0) {
// printf("input.c: %s\t(%u)\n", kbd->keydown ? "down" : "up", kbd->keycode);
//   }
}
