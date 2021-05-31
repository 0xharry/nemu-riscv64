// J(U)-type:
// x[rd] = pc+4; pc += sext(offset)
static inline sword_t jal_offset_decode(word_t imm)
{
  sword_t simm = ((sword_t)(
                ((imm & (word_t)0x00080000)<<1 ) |\
                ((imm & (word_t)0x000000FF)<<12) |\
                ((imm & (word_t)0x00000100)<<3 ) |\
                ((imm & (word_t)0x0007FE00)>>8 )))<<43>>43;
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

static inline def_EHelper(jal)
{
  rtl_addi(s, ddest, &s->seq_pc, 0); // x[rd] = pc+4
  rtl_j(s, cpu.pc + jal_offset_decode(id_src1->imm));   // pc += sext(offset)
  printf("%lx\t%lx\t%lx\n",jal_offset_decode(id_src1->imm), cpu.pc, s->jmp_pc);
  print_asm_template2(jal);
}

// // j expands to jal x[$0], offset
// static inline def_EHelper(j)
// {
//   // pc += sext(offset)
//   print_asm_template2(j);
// }

// J(I)-type:
// t =pc+4
// pc=(x[rs1]+sext(offset))&~1
// x[rd]=t
static inline def_EHelper(jalr)
{
  rtl_j(s, (*dsrc1+id_src2->imm)&~1);
  rtl_addi(s, ddest, rz, s->seq_pc);
  print_asm_template2(jalr);
}

// // ret expands to jalr x0, 0(x1)
// static inline def_EHelper(ret)
// {
//   // pc = x[1]
//   print_asm_template2(ret);
// }