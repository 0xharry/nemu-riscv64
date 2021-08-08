#include <am.h>
#include <nemu.h>
// #include <sys/time.h>
// #include <time.h>

uint32_t high,low;

void __am_timer_init() {
    high = inl(0xa100004c);
    low  = inl(0xa1000048);
}

// static struct timeval boot_time = {};

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
    uint32_t cur_high = inl(0xa100004c);
    uint32_t cur_low  = inl(0xa1000048);
    uptime->us = (cur_high-high) * 1000000 + (cur_low-low + 500);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
