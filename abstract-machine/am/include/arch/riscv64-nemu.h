#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  uintptr_t gpr[32], cause, status, epc;
  void *pdir;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[0]
#define GPR3 gpr[0]
#define GPR4 gpr[0]
#define GPRx gpr[0]

#endif

/*
  -> CONTEXT_SIZE
.
.
.
sp-> 0

t0, scause
t1, sstatus
t2, sepc

*/