// U-type: x[rd] = sext(immediate[31:12] << 12)
static inline def_EHelper(lui)
{
  rtl_li(s, ddest, id_src1->imm); // id_src1->imm 'imm' cannot be substituted by dsrc1/2 before calling rtl_li() // actually not???
  print_asm_template2(lui);
}

// I-type: x[rd] = x[rs1] + sext(immediate)
static inline def_EHelper(addi)
{
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(addi);
}

// // mv expands to addi rd, rs1, 0
// static inline def_EHelper(mv)
// {
//   // x[rd] = x[rs1]
//   print_asm_template2(mv);
// }

// // expands to instr sequence: lui, addi, slli, addi, slli, addi ,slli, addi (start with addi)
// static inline def_EHelper(li)
// {
//   // x[rd] = immediate
//   print_asm_template2(li);
// }

// U-type: x[rd] = pc + sext(immediate[31:12] << 12)
static inline def_EHelper(auipc)
{
  rtl_li(s, ddest, cpu.pc + id_src1->imm); // 'pc' in "pc+imm" means cpu.pc or seq_pc (has been + 4) ???
  print_asm_template2(auipc);
}