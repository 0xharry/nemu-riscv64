#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  union {
    void *pdir;
    uintptr_t gpr[32];
  };
  uintptr_t cause;
  uintptr_t status;
  uintptr_t epc;
};


#define GPR1 gpr[17]  // a7
#define GPR2 gpr[10]  // a0
#define GPR3 gpr[11]  // a1
#define GPR4 gpr[12]  // a2 
#define GPRx gpr[10]  // a0

#endif
