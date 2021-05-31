// J(U)-type:
// x[rd] = pc+4; pc += sext(offset)
static inline def_EHelper(jal)
{
  rtl_addi(s, ddest, &s->seq_pc, 0); // x[rd] = pc+4
  rtl_j(s, cpu.pc + id_src1->imm);   // pc += sext(offset)
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