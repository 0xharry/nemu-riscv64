#include <am.h>
#include <nemu.h>

void __am_timer_init() {
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = 0;
  // uptime->us = seconds * 1000000 + (useconds + 500);
  uint32_t usec = inl(RTC_ADDR);
  uint32_t sec = inl(RTC_ADDR+4);
  uptime->us = sec * 1000000 + (usec-3000);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 6;
  rtc->minute = 6;
  rtc->hour   = 0;
  rtc->day    = 9;
  rtc->month  = 8;
  rtc->year   = 1998;
}
