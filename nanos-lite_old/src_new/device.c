#include <common.h>
#include <sys/time.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  int i;
  for(i=0;i<len;i++) {
    putch(((char *)buf)[i]); 
  }
  return len;
}


// return 0 for success, or -1 for failure
// AM_TIMER_UPTIME, AM系统启动时间, 可读出系统启动后的微秒数.
// #define io_read(reg) 
//   ({ reg##_T __io_param; 
//     ioe_read(reg, &__io_param); 
//     __io_param; })
int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
  tv->tv_sec =  io_read(AM_TIMER_UPTIME).us/1000000;
  tv->tv_usec = io_read(AM_TIMER_UPTIME).us%1000000;
  return 0;
}

//refer to amkernels
size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE) {
    memset(buf,0,len);
    return 0;
  }
  return sprintf(buf, "%s %s\n", ev.keydown ? "kd" : "ku",keyname[ev.keycode]);
}

int width = 0;
int height= 0;

//将文件的len字节写到buf中 WIDTH : 640  HEIGHT:480
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  //printf("dispinfo_read start\n");
  AM_GPU_CONFIG_T gpu_config = io_read(AM_GPU_CONFIG);
  printf("w : %d, h : %d\n",gpu_config.width,gpu_config.height);
  width = gpu_config.width;
  height = gpu_config.height;
  return sprintf((char *)buf,"WIDTH : %d\nHEIGHT : %d\n",gpu_config.width,gpu_config.height);
}

char dispinfo[128];
// size_t (*WriteFn) (const void *buf, size_t offset, size_t len);
// 写入大小信息
size_t dispinfo_write(const void *buf,size_t offset, size_t len) {
  //printf("get in dispinfo_write\n");
  strncpy(dispinfo,buf,len);
  return strlen(dispinfo);
}

extern size_t fs_lseek(int fd, size_t offset, int whence) ;

static int getWidth() {
  int i;
  int sum = 0;
  for(i=7;dispinfo[i]!='\n';i++) {
    sum = sum*10 + dispinfo[i]-'0';
  }
  printf("sum is %d\n",sum);
  return sum;
}
/*
 用于把buf中的len字节写到屏幕上offset处. 你需要先从offset计算出屏幕上的坐标, 
 然后调用IOE来进行绘图. 另外我们约定每次绘图后总是马上将frame buffer中的内容同步到屏幕上.*/
size_t fb_write(const void *buf, size_t offset, size_t len) {
  // printf("get in fb_write\n");
  // sscanf(dispinfo, "WIDTH: %d\nHEIGHT: %d\n", &width, &height);
  width = getWidth();
  offset = fs_lseek(5,0,1);
  int x = offset/4%width;
  int y = offset/4/width;
  //AM_DEVREG(11, GPU_FBDRAW,   WR, int x, y; void *pixels; int w, h; bool sync);
  //    io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
  io_write(AM_GPU_FBDRAW, x, y, (uint32_t *)buf, width, 1, true);
  // printf("get out of fb_write\n");
  return len;
}


void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
