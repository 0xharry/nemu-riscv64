#ifndef __CPU_DECODE_H__
#define __CPU_DECODE_H__

#include <isa.h>

#define OP_STR_SIZE 40
enum { OP_TYPE_REG, OP_TYPE_MEM, OP_TYPE_IMM };

typedef struct {
  uint32_t type;
  int width;
  union {
    uint32_t reg;
    word_t imm;
    sword_t simm;
  };
  rtlreg_t *preg;
  rtlreg_t val;
  char str[OP_STR_SIZE];
} Operand;

// DecodeExecState: "Dashboard" of this state machine
typedef struct {
  uint32_t opcode; // raw instruction
  vaddr_t seq_pc;  // sequential pc
  uint32_t is_jmp;
  vaddr_t jmp_pc;
  Operand src1, dest, src2; // eg. src1=rs1, src2=imm, dest=r[..]
  int width;  // operand width (might be 64)
  rtlreg_t tmp_reg[4]; // s0 s1 s2 t0
  ISADecodeInfo isa; // raw instruction break down
} DecodeExecState;

#define def_DHelper(name) void concat(decode_, name) (DecodeExecState *s)

#ifdef DEBUG
#define print_Dop(...) snprintf(__VA_ARGS__)
#else
#define print_Dop(...)
#endif

#endif
