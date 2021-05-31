// U-type: x[rd] = sext(immediate[31:12] << 12)
static inline def_EHelper(lui)
{
  rtl_li(s, ddest, *dsrc2); // id_src1->imm 'imm' cannot be substituted by dsrc1/2 before calling rtl_li() // actually not???
  print_asm_template2(lui);
}

// I-type: x[rd] = x[rs1] + sext(immediate)
static inline def_EHelper(addi)
{
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(addi);
}

// x[rd] = immediate
// expands to instr sequence: lui, addi, slli, addi, slli, addi ,slli, addi
static inline def_EHelper(li)
{
  // rtl_li(s, ddest, id_src1->imm);
  // print_asm_template2(li);
}

// x[rd] = pc + sext(immediate[31:12] << 12)
static inline def_EHelper(auipc)
{
  rtl_li(s, ddest, id_src1->imm); // 'pc' in "pc+imm" means cpu.pc or seq_pc (has been + 4) ???
  print_asm_template2(auipc);
}