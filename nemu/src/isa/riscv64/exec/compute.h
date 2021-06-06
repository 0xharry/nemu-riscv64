// U-TYPE:-------------------------------------
// U-type: x[rd] = sext(immediate[31:12] << 12)
static inline def_EHelper(lui)
{
  rtl_li(s, ddest, id_src1->imm); // id_src1->imm 'imm' cannot be substituted by dsrc1/2 before calling rtl_li() // actually not???
  print_asm_template2(lui);
}

// U-type: x[rd] = pc + sext(immediate[31:12] << 12)
static inline def_EHelper(auipc)
{
  rtl_li(s, ddest, cpu.pc + id_src1->imm); // 'pc' in "pc+imm" means cpu.pc or seq_pc (has been + 4) ???
  print_asm_template2(auipc);
}


// I-TYPE:---------------------------------------
// ddest, dsrc1, id_src2=simm

// I-type: x[rd] = (x[rs1] <𝑢 sext(immediate))
static inline def_EHelper(sltiu)
{
  *ddest = ((word_t)*dsrc1 < (word_t)id_src2->simm);
  print_asm_template2(sltiu);
}

// I-type: x[rd] = x[rs1] + sext(immediate)
static inline def_EHelper(addi)
{
  rtl_addi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(addi);
}

// I-type: x[rd] = sext((x[rs1] + sext(immediate))[31:0])
static inline def_EHelper(addiw)
{
  rtl_addwi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(addiw);
}

// x[rd] = (x[rs1] ≫ 𝑠 shamt)
static inline def_EHelper(srai)
{
  // shamt[5]=0 时指令有效
  // rtl_sari自带mask
  rtl_sari(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(srai);
}

// x[rd] = (x[rs1] ≫ 𝑠 shamt)
static inline def_EHelper(srli)
{
  rtl_shli(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(srli);
}

// x[rd] = x[rs1] ≪ shamt
static inline def_EHelper(slli)
{
  rtl_shli(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(slli);
}

// x[rd] = x[rs1] ^ sext(immediate)
static inline def_EHelper(xori)
{
  rtl_xori(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(xori);
}

// x[rd] = x[rs1] | sext(immediate)
static inline def_EHelper(ori)
{
  rtl_ori(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(ori);
}

// x[rd] = x[rs1] & sext(immediate)
static inline def_EHelper(andi)
{
  rtl_andi(s, ddest, dsrc1, id_src2->imm);
  print_asm_template2(andi);
}


// R-TYPE----------------------------------------
// ddest, dsrc1, dsrc2

// R-type: x[rd] = sext((x[rs1] + x[rs2])[31:0])
static inline def_EHelper(addw)
{
  rtl_addw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(addw);
}

// R-type: x[rd] = x[rs1] − x[rs2]
static inline def_EHelper(sub)
{
  rtl_sub(s, ddest, dsrc1, dsrc2);
  print_asm_template3(sub);
}

// R-type: x[rd] = x[rs1] + x[rs2]
static inline def_EHelper(add)
{
  rtl_add(s, ddest, dsrc1, dsrc2);
  print_asm_template3(add);
}

// x[rd] = x[rs1] ≪ x[rs2]
static inline def_EHelper(sll)
{
  rtl_shl(s, ddest, dsrc1, dsrc2);
  print_asm_template3(sll);
}

// x[rd] = (x[rs1] < 𝑠 x[rs2])
static inline def_EHelper(slt)
{
  rtl_li(s, ddest, ((int32_t)*dsrc1<(int32_t)*dsrc2));
  print_asm_template3(slt);
}

// x[rd] = (x[rs1] < 𝑢 x[rs2])
static inline def_EHelper(sltu)
{
  rtl_li(s, ddest, ((uint32_t)*dsrc1<(uint32_t)*dsrc2));
  print_asm_template3(sltu);
}

// x[rd] = x[rs1] ^ x[rs2]
static inline def_EHelper(xor)
{
  rtl_xor(s, ddest, dsrc1, dsrc2);
  print_asm_template3(xor);
}

// x[rd] = (x[rs1] ≫ 𝑢 x[rs2])
static inline def_EHelper(srl)
{
  rtl_shr(s, ddest, dsrc1, dsrc2);
  print_asm_template3(srl);
}

// x[rd] = x[rs1] | 𝑥[𝑟𝑠2]
static inline def_EHelper(or)
{
  rtl_or(s, ddest, dsrc1, dsrc2);
  print_asm_template3(or);
}

// x[rd] = x[rs1] & 𝑥[𝑟𝑠2]
static inline def_EHelper(and)
{
  rtl_and(s, ddest, dsrc1, dsrc2);
  print_asm_template3(and);
}

// mv: expands to addi rd, rs1, 0
// x[rd] = x[rs1]

// li: expands to instr sequence: lui, addi, slli, addi, slli, addi ,slli, addi (start with addi)
// x[rd] = immediate

// seqz: expands to sltiu rd, rs1, 1
// x[rd] = (x[rs1] == 0)