#ifndef __ISA_RISCV64_H__
#define __ISA_RISCV64_H__

#include <common.h>

// memory
#define riscv64_IMAGE_START 0x100000
#define riscv64_PMEM_BASE 0x80000000

// Control and Status Registers
typedef struct {
  vaddr_t sepc;
  uintptr_t scause;
  vaddr_t stvec;
  union {
    struct {
      uint32_t UIE   : 1;
      uint32_t SIE   : 1;
      uint32_t NOP1  : 2;
      uint32_t UPIE  : 1;
      uint32_t SPIE  : 1;
      uint32_t NOP2  : 2;
      uint32_t SPP   : 1;
      uint32_t NOP3  : 4;
      uint32_t FS    : 2;
      uint32_t XS    : 2;
      uint32_t NOP4  : 1;
      uint32_t SUM   : 1;
      uint32_t MXR   : 1;
      uint32_t XLEN2 :21;
      uint32_t XLEN1 : 1;
    } bit;
    uint32_t val;
  } sstatus;
} CSR;

// reg
typedef struct {
  union {
    rtlreg_t _64;
  } gpr[32]; // 32个通用register

  vaddr_t pc;
  CSR csr;
} riscv64_CPU_state;

// decode
typedef struct {
  union {
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      int32_t  simm11_0  :12;
    } i;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm4_0    : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      int32_t  simm11_5  : 7;
    } s;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t funct7    : 7;
    } r;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      int32_t  simm31_12 :20;
    } u;
    struct {
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm11     : 1;
      uint32_t imm4_1    : 4;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t imm10_5   : 6;
      int32_t  simm12    : 1;
    } b;
    uint32_t val; // instruction (binary)
  } instr;
} riscv64_ISADecodeInfo;

#define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
#define riscv64_has_mem_exception() (cpu.mem_exception != 0)

#endif
