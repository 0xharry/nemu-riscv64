#ifndef __ISA_RISCV64_H__
#define __ISA_RISCV64_H__

#include <common.h>

// memory
#define riscv64_IMAGE_START 0x100000
// riscv物理地址从0x80000000开始
#define riscv64_PMEM_BASE 0x80000000

// reg

typedef struct {
  union {
    rtlreg_t _64;
  } gpr[32];

  vaddr_t pc;

  //S-mode CSR
  vaddr_t sepc; //保存当前pc值
  vaddr_t scause;//设置异常号
  vaddr_t stvec; //保存异常入口地址
  vaddr_t sscratch;
  vaddr_t stval;

  union{
    struct{
      uint32_t uie     : 1;
      uint32_t sie     : 1;
      uint32_t pad2_3  : 2;
      uint32_t upie    : 1;
      uint32_t spie    : 1;
      uint32_t pad6_7  : 2;
      uint32_t spp     : 1;
      uint32_t pad9_12 : 4;
      uint32_t FS      : 2;
      uint32_t XS      : 2;
    };
    uint32_t val;
  }sstatus;

  //M-mode CSR 
  //M 模式下运行时，只有在全局中断使能位 mstatus.MIE 置 1 时才会产生中断.
  //vaddr_t mtvec,mepc,mcause,mie,mip,mtval,msrcatch,mstatus;

  //csrs一共有12位，共4096个
  //vaddr_t CSRS[4096];
 
} riscv64_CPU_state;

// decode 立即数都是有符号数
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
      int32_t  simm31_12 :20;
    } u;

    //implement more type
    //这些格式的立即数字段总是符号扩展，符号位总是在指令中最高位！！！
    struct{
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t funct7    : 7;
    }r;

    struct{
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t imm11     : 1;
      uint32_t imm4_1    : 4;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      uint32_t rs2       : 5;
      uint32_t imm10_5   : 6;
      int32_t  simm12    : 1;
    }b;


    struct{
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t imm19_12  : 8; 
      uint32_t imm11     : 1;
      uint32_t imm10_1   : 10;
      int32_t  simm20    : 1;
    }j;

    struct{
      uint32_t opcode1_0 : 2;
      uint32_t opcode6_2 : 5;
      uint32_t rd        : 5;
      uint32_t funct3    : 3;
      uint32_t rs1       : 5;
      //寄存器编号，使用无符号数
      uint32_t csr       : 12;
    }csr;
    uint32_t val;
  } instr;
} riscv64_ISADecodeInfo;

#define isa_vaddr_check(vaddr, type, len) (MEM_RET_OK)
#define riscv64_has_mem_exception() (cpu.mem_exception != 0)

#endif
