#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    EXW  (3, ld, 8)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (3, st, 8)
    default: exec_inv(s);
  }
}

static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)  // case 0b00000: set_width(s, 0); decode_I(s); exec_load(s); break;
    IDEX (0b00100, I, addi)  // case 0b00000: set_width(s, 0); decode_I(s); exec_addi(s); break;
    IDEX (0b01000, S, store) // case 0b01000: set_width(s, 0); decode_S(s); exec_store(s); break;
    IDEX (0b01101, U, lui)   // case 0b01101: set_width(s, 0); decode_U(s); exec_lui(s); break;
    IDEX (0b00101, U, auipc) // case 0b00101: set_width(s, 0); decode_U(s); exec_auipc(s); break;
    IDEX (0b11011, U, jal)
    IDEX (0b11001, U, jalr)
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
