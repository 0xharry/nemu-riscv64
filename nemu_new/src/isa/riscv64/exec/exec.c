#include <cpu/exec.h>
#include "../local-include/decode.h"
#include "all-instr.h"

static inline void set_width(DecodeExecState *s, int width) {
  if (width != 0) s->width = width;
}

static inline def_EHelper(load) {
  switch (s->isa.instr.i.funct3) {
    //lws这个命名有问题，但是懒得改了
    EXW  (0, lws, 1)
    EXW  (1, lws, 2)
    EXW  (2, lws, 4)
    EXW  (3, ld,  8)
    EXW  (4, ld,  1)  //从一个字节加载 lbu
    EXW  (5, ld,  2)  //lhu
    EXW  (6, ld,  4)
    default: exec_inv(s);
  }
}

static inline def_EHelper(store) {
  switch (s->isa.instr.s.funct3) {
    EXW  (0, st, 1)
    EXW  (1, st, 2)  //只存低两位
    EXW  (2, st, 4)  //sw 低4字节
    EXW  (3, st, 8)

    default: exec_inv(s);
  }
}

static inline def_EHelper(i32) {
  if(s->isa.instr.i.funct3!=5)
  switch (s->isa.instr.i.funct3) {
    EX (0, addiw)
    EX (1, slliw)
    // EX (5, srliw)
    // EX (5, sraiw)
    default: exec_inv(s);
  }
  else{
    switch (s->isa.instr.i.simm11_0>>5) {
      EX (0, srliw)
      EX (0b0100000,sraiw)
    }
  }
}

static inline def_EHelper(r2rw){
  switch (s->isa.instr.r.funct3 | (s->isa.instr.r.funct7<<3)) {
    EX (0x000, addw)
    EX (0x100, subw)
    EX (0x105, sraw)
    EX (0x001, sllw)
    EX (0x005, srlw)
    EX (0x008, mulw)
    EX (0x00c, divw)
    EX (0x00d, divuw)
    EX (0x00e, remw)
    EX (0x00f, remuw)
    default: exec_inv(s);
  }
}

static inline def_EHelper(r2r){
  switch (s->isa.instr.r.funct3 | (s->isa.instr.r.funct7<<3)) {
    EX (0x000, add)
    EX (0x100, sub)
    EX (0x001, sll)
    EX (0x002, slt)
    EX (0x003, sltu)
    EX (0x004, xor)
    EX (0x005, srl)
    EX (0x105, sra)
    EX (0x006, or)
    EX (0x007, and)
    EX (0x008, mul)
    EX (0x00d, divu)
    EX (0x00f, remu)
    default: exec_inv(s);
  }
}

static inline def_EHelper(r2i){
  if(s->isa.instr.i.funct3!=5)
  switch (s->isa.instr.i.funct3) {
    EX (0, addi)
    EX (1, slli)
    EX (2, slti)
    EX (3, sltiu)
    EX (4, xori)
    EX (6, ori)
    EX (7, andi)
    default: exec_inv(s);
  }else{
    switch (s->isa.instr.i.simm11_0>>6){
      EX(0x00, srli)
      EX(0x10, srai)
    }
  }
}

static inline def_EHelper(branch){
  switch (s->isa.instr.b.funct3) {
    EX (0, beq)
    EX (1, bne)
    EX (4, blt)
    EX (5, bge)
    EX (6, bltu)
    EX (7, bgeu)
    default: exec_inv(s);
  }
}

static inline def_EHelper(system) {
  switch (s->isa.instr.i.funct3) {
    IDEX  (0, CSR, inter) //ecall sret ebreak需要分类讨论
    IDEX  (1, CSR, csrrw)
    IDEX  (2, CSR, csrrs)
    IDEX  (3, CSR, csrrc)
    IDEX  (5, CSRI, csrrwi)  
    IDEX  (6, CSRI, csrrsi)  
    IDEX  (7, CSRI, csrrci)
    default: exec_inv(s);
  }
}



static inline void fetch_decode_exec(DecodeExecState *s) {
  s->isa.instr.val = instr_fetch(&s->seq_pc, 4);
  //opcode第0 1 位都必须为1
  Assert(s->isa.instr.i.opcode1_0 == 0x3, "Invalid instruction");
  switch (s->isa.instr.i.opcode6_2) {
    IDEX (0b00000, I, load)
    IDEX (0b01000, S, store)
    IDEX (0b01101, U, lui)
    //TODO: add decode of R and B
    IDEX (0b01110, R, r2rw)
    IDEX (0b01100, R, r2r)
    IDEX (0b11000, B, branch)
    IDEX (0b00101, U, auipc)
    IDEX (0b11011, J, jal)
    IDEX (0b00100, I, r2i)
    IDEX (0b00110, I, i32)
    // IDEX (0b00110, I, addiw)
    EX   (0b11010, nemu_trap)
    IDEX (0b11001, I, jalr)
    IDEX (0b11100, CSR, system)
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

