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
    EXW  (0b011, ld, 8) // ld
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

static inline def_EHelper(I_type)
{
  switch (s->isa.instr.i.funct3)
  {
    EX(0b000, addi)
    // EX(0b010, slli)
    EX(0b011, sltiu)
    // EX(0b100, xori)
    // EX(0b101, srxl) // srli or srai
    // EX(0b110, ori)
    // EX(0b111, andi)
    default: exec_inv(s);
  }
}

// static inline def_EHelper(srxl) {
//   switch (id_src2->imm>>5)
//   {
//     EX(0b0000000, srli)
//     EX(0b0100000, srai)
//     default: exec_inv(s);
//   }
// }


static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)  // case 0b00000: set_width(s, 0); decode_I(s); exec_load(s); break;
    IDEX (0b00100, I, I_type)  // case 0b00000: set_width(s, 0); decode_I(s); exec_I_type(s); //second decode; break;
    IDEX (0b00110, I, addiw)
    IDEX (0b01110, S, addw)
    IDEX (0b01100, S, sub)
    IDEX (0b11000, B, bne)
    IDEX (0b01000, S, store) // case 0b01000: set_width(s, 0); decode_S(s); exec_store(s); break;
    IDEX (0b01101, U, lui)   // case 0b01101: set_width(s, 0); decode_U(s); exec_lui(s); break;
    IDEX (0b00101, U, auipc) // case 0b00101: set_width(s, 0); decode_U(s); exec_auipc(s); break;
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
