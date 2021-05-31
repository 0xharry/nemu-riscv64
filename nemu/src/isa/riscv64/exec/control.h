// static inline def_EHelper(j)
// {
//   rtl_li(s, ddest, id_src1->imm);
//   print_asm_template2(j);
// }

// static inline def_EHelper(jal)
// {
//   rtl_li(s, ddest, id_src1->imm);
//   print_asm_template2(jal);
// }

// t =pc+4
// pc=(x[rs1]+sext(offset))&~1
// x[rd]=t
static inline def_EHelper(jalr)
{
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(jalr);
}

// pc = x[1]
// expands to jalr x0, 0(x1)
static inline def_EHelper(ret)
{
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(ret);
}