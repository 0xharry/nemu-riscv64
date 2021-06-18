#include <am.h>
#include <nemu.h>

//#define MODE_800x600
#ifdef MODE_800x600
# define W    800
# define H    600
#else
# define W    400
# define H    300
#endif

#define SYNC_ADDR (VGACTL_ADDR + 4)
uint32_t *fb;
static inline int min(int x, int y) { return (x < y) ? x : y; }

void __am_gpu_init() {
  // int i;
  // int w = W;
  // int h = H;
  // fb = (uint32_t *)(uintptr_t)FB_ADDR;
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

// sync==1时，向屏幕(x,y)坐标处绘制w*h的矩形图像。
// 图像像素按行优先方式存储在pixels中, 每个像素用32位整数以00RRGGBB的方式描述颜色.
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  int pixels_w = min(w, W - x); // pixels 一行的大小(*4B)
  for (int j = 0; j < h && y + j < H; j ++) { // copy h行
    for(int bias=0; bias<pixels_w; ++bias) {
      outb(FB_ADDR + (y + j) * W + x + bias, pixels[bias]);
    }
    pixels += w;
  }

  outl(SYNC_ADDR, 1);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
