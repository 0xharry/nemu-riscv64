static inline sword_t jal_offset_decode(word_t imm)
{
  sword_t simm = ((sword_t)(((imm & (word_t)0x00080000) << 1) |
                            ((imm & (word_t)0x000000FF) << 12)|
                            ((imm & (word_t)0x00000100) << 3) |
                            ((imm & (word_t)0x0007FE00) >> 8)))
                  << 43 >> 43; //sizeof(word_t)-21 = 64-21 = 43
  return simm;
}
/* offset[20|10:1|11|19:12]
 * 20   16   12   8    4    0
 * 0 0000 0000 0000 0000 0000
 * 0                1111 1111 << 12
 * 0              1 0000 0000 << 3
 * 0  111 1111 1110 0000 0000 >> 8
 * 0 1                        << 1
 */

// J-TYPE:
// J(U)-type:
// x[rd] = pc+4; pc += sext(offset)
static inline def_EHelper(jal)
{
  rtl_addi(s, ddest, &s->seq_pc, 0);                  // x[rd] = pc+4
  rtl_j(s, cpu.pc + jal_offset_decode(id_src1->imm)); // pc += sext(offset)
  print_asm_template2(jal);
}

// J(I)-type:
// t =pc+4
// pc=(x[rs1]+sext(offset))&~1
// x[rd]=t
static inline def_EHelper(jalr)
{
  rtl_j(s, (*dsrc1 + id_src2->imm) & ~1);
  rtl_addi(s, ddest, rz, s->seq_pc);
  print_asm_template2(jalr);
}



// B_TYPE: -------------------------------
// id_src1=rs1, id_src2=rs2, id_dest=imm

// if (rs1 ≠ rs2) pc += sext(offset) (pc changed in decode)
static inline def_EHelper(bne)
{
  rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(bne);
}

// if (rs1 == rs2) pc += sext(offset)
static inline def_EHelper(beq)
{
  rtl_jrelop(s, RELOP_EQ, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(beq);
}

// if (rs1 < s rs2) pc += sext(offset)
static inline def_EHelper(blt)
{
  rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(blt);
}

// if (rs1 < u rs2) pc += sext(offset)
static inline def_EHelper(bltu)
{
  rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(bltu);
}

// if (rs1 ≥ s rs2) pc += sext(offset)
static inline def_EHelper(bge)
{
  rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(bge);
}

// if (rs1 ≥ u rs2) pc += sext(offset)
static inline def_EHelper(bgeu)
{
  rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template2(bgeu);
}

// EXTENDS:
// j: expands to jal x[$0], offset
// pc += sext(offset)

// ret: expands to jalr x0, 0(x1)
// pc = x[1]