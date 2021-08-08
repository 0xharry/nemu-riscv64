#include <am.h>
#include <nemu.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)

#ifdef MODE_800x600
# define W    800
# define H    600
#else
# define W    400
# define H    300
#endif


int N = 32;


void __am_gpu_init() {
  // int i;
  // int w = W;
  // int h = H;
  // uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  // for (i = 0; i < w * h; i ++) fb[i] = i;
  // outl(SYNC_ADDR, 1);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W, .height = H,
    .vmemsz = 0
  };
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  outl(SYNC_ADDR, 1);
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  //用于写入的vga虚存地址 
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  //从坐标(x,y)开始写
  for (int i = 0; i < h && y + i < H; i ++) {
    for (int j = 0; j < w && x + j < W; j ++) {
      fb[(i + y) * W + j + x] = *pixels;
      pixels++;
    }
  }
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}