#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = 0;
  // uptime->us = seconds * 1000000 + (useconds + 500);
  outb(RTC_ADDR,1);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 9;
  rtc->month  = 8;
  rtc->year   = 1998;
}
