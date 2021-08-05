#include <device/map.h>
#include <device/alarm.h>
#include <monitor/monitor.h>
#include <sys/time.h>
//i8253计时器初始化时会分别注册0x48处长度为8个字节的端口, 
//以及0xa1000048处长度为8字节的MMIO空间, 它们都会映射到RTC寄存器. CPU可以访问这两个寄存器来获得当前时间.
#define RTC_PORT 0x48   // Note that this is not the standard
#define RTC_MMIO 0xa1000048

static uint32_t *rtc_port_base = NULL;

//获得当前时间
static void rtc_io_handler(uint32_t offset, int len, bool is_write) {
  assert(offset == 0 || offset == 4);
  if (!is_write) {
    struct timeval now;
    gettimeofday(&now, NULL);
    rtc_port_base[0] = now.tv_usec;
    rtc_port_base[1] = now.tv_sec;
  }
}
//时钟中断
static void timer_intr() {
  if (nemu_state.state == NEMU_RUNNING) {
    extern void dev_raise_intr();
    dev_raise_intr();
  }
}

//初始化时钟
void init_timer() {
  rtc_port_base = (void*)new_space(8);
  add_pio_map("rtc", RTC_PORT, (void *)rtc_port_base, 8, rtc_io_handler);
  add_mmio_map("rtc", RTC_MMIO, (void *)rtc_port_base, 8, rtc_io_handler);
  add_alarm_handle(timer_intr);
}
