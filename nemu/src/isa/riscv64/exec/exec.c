#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (0b000, ld, 1) // lb
    EXW  (0b001, ld, 2) // lh
    EXW  (0b010, ld, 4) // lw
    EXW  (0b011, ldu,8) // ld
    EXW  (0b100, ldu,1) // lbu
    EXW  (0b101, ldu,2) // lhu
    EXW  (0b110, ldu,4) // lwu
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (0b000, st, 1) // sb
    EXW  (0b001, st, 2) // sh
    EXW  (0b010, st, 4) // sw
    EXW  (0b011, st, 8) // sd
    default: exec_inv(s);
  }
}


static inline def_EHelper(srxl) {
  switch (id_src2->imm>>6) {
    EX(0b000000, srli)
    EX(0b010000, srai)
    default: exec_inv(s);
  }
}
static inline def_EHelper(I_type_a) {
  switch (s->isa.instr.i.funct3) {
    EX(0b000, addi)
    EX(0b001, slli)
    EX(0b010, slti)
    EX(0b011, sltiu)
    EX(0b100, xori)
    EX(0b101, srxl) // srli or srai
    EX(0b110, ori)
    EX(0b111, andi)
    default: exec_inv(s);
  }
}
static inline def_EHelper(slra_liw) {
  switch (id_src2->imm>>6) {
    EX(0b000000, srliw)
    EX(0b010000, sraiw)
    default: exec_inv(s);
  }
}
static inline def_EHelper(I_type_b) {
  switch (s->isa.instr.i.funct3) {
    EX(0b000, addiw)
    EX(0b001, slliw)
    EX(0b101, slra_liw)
    default: exec_inv(s);
  }
}


static inline def_EHelper(B_type) {
  switch (s->isa.instr.b.funct3) {
    EX(0b000, beq)
    EX(0b001, bne)
    EX(0b100, blt)
    EX(0b101, bge)
    EX(0b110, bltu)
    EX(0b111, bgeu)
    default: exec_inv(s);
  }
}


static inline def_EHelper(add_sub) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, add)
    EX(0b0000001, mul)
    EX(0b0100000, sub)
  default: exec_inv(s);
  }
}
static inline def_EHelper(xor_div) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, xor)
    EX(0b0000001, div)
  default: exec_inv(s);
  }
}
static inline def_EHelper(srl_divu) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, srl)
    EX(0b0000001, divu)
  default: exec_inv(s);
  }
}
static inline def_EHelper(or_rem) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, or)
    EX(0b0000001, rem)
  default: exec_inv(s);
  }
}
static inline def_EHelper(and_remu) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, and)
    EX(0b0000001, remu)
  default: exec_inv(s);
  }
}
static inline def_EHelper(sll_mulh) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, sll)
    EX(0b0000001, mulh)
  default: exec_inv(s);
  }
}
static inline def_EHelper(slt_mulhsu) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, slt)
    EX(0b0000001, mulhsu)
  default: exec_inv(s);
  }
}
static inline def_EHelper(sltu_mulhu) {
  switch (s->isa.instr.r.funct7) {
    EX(0b0000000, sltu)
    EX(0b0000001, mulhu)
  default: exec_inv(s);
  }
}
static inline def_EHelper(R_type_a) {
  switch (s->isa.instr.r.funct3) {
    EX(0b000, add_sub)
    EX(0b001, sll_mulh)
    EX(0b010, slt_mulhsu)
    EX(0b011, sltu_mulhu)
    EX(0b100, xor_div)
    EX(0b101, srl_divu)
    EX(0b110, or_rem)
    EX(0b111, and_remu)
    default: exec_inv(s);
  }
}

static inline def_EHelper(addw_subw) {
  switch (s->isa.instr.r.funct7) {
    EX(0x00, addw)
    EX(0b01, mulw)
    EX(0x20, subw)
  default: exec_inv(s);
  }
}
static inline def_EHelper(srlw_sraw) {
  switch (s->isa.instr.r.funct7) {
    EX(0x00, srlw)
    EX(0x01, divuw)
    EX(0x20, sraw)
  default: exec_inv(s);
  }
}
static inline def_EHelper(R_type_b) {
  switch (s->isa.instr.r.funct3) {
    EX(0b000, addw_subw)
    EX(0b001, sllw)
    EX(0b100, divw)
    EX(0b101, srlw_sraw)
    EX(0b110, remw)
    EX(0b111, remuw)
    default: exec_inv(s);
  }
}


static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)  // case 0b00000: set_width(s, 0); decode_I(s); exec_load(s); break;
    IDEX (0b00100, I, I_type_a)  // case 0b00000: set_width(s, 0); decode_I(s); exec_I_type_a(s); //second decode; break;
    IDEX (0b00110, I, I_type_b)
    IDEX (0b01100, R, R_type_a)
    IDEX (0b01110, R, R_type_b)
    IDEX (0b11000, B, B_type)
    IDEX (0b01000, S, store)
    IDEX (0b01101, U, lui)
    IDEX (0b00101, U, auipc)
    IDEX (0b11011, J, jal)
    IDEX (0b11001, I, jalr)
    EX   (0b11010, nemu_trap)
    // TODO(); more instructions id+ex
    default: exec_inv(s);
  }
}

static inline void reset_zero() {
  reg_d(0) = 0;
}

vaddr_t isa_exec_once() {
  DecodeExecState s;
  s.is_jmp = 0;
  s.seq_pc = cpu.pc;

  fetch_decode_exec(&s);
  update_pc(&s);

  reset_zero();

  return s.seq_pc;
}
