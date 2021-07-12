#ifdef _KLIB_TEST_
#include "../tests/klib_test.h"
#include "../include/klib-macros.h"
#else
#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#endif


#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

// convert a string to an integer
int atoi(const char* nptr) {
  int x = 0, neg_flag=1;
  while (*nptr == ' ') { nptr ++; }
  if(*nptr == '-') { neg_flag=-1; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x*neg_flag;
}

static uintptr_t brk_klib = 0;
void *malloc(size_t size) {
  void *start = heap.start+brk_klib;
  // page aligned 8 Byte;
  brk_klib += ROUNDUP(size, 8);
  // overflow may happen
  if((uintptr_t)heap.start + brk_klib > (uintptr_t)heap.end)
    return NULL;

  for(char *flush=start; flush != start+brk_klib; ++flush)
    *flush=0;
// printf("malloc %d in %p\n", ROUNDUP(size, 8), start);
  return start;
}

void free(void *ptr) {
}

#endif
