//将立即数写入高20位，清除低12位
static inline def_EHelper(lui) {
  rtl_li(s, ddest, id_src1->imm);
  print_asm_template2(lui);
}

//把符号位扩展的 20 位（左移 12 位）立即数加到 pc 上，结果写入 x[rd]。
//decode已经将imm译码出来了
static inline def_EHelper(auipc) {
  *s0 = cpu.pc + id_src1->imm;
  rtl_li(s,ddest,*s0);
  print_asm_template2(auipc);
}

//将立即数写入reg,不区分高20和低12
//rd = rs1 + imm
static inline def_EHelper(addi) {
  rtl_addi(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(addi);
}

static inline def_EHelper(add) {
  rtl_add(s,ddest,dsrc1,dsrc2);
  print_asm_template3(add);
}

//x[rd] = sext((x[rs1] + sext(immediate))[31:0])
static inline def_EHelper(addiw) {
  rtl_addiw(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(addiw);
}


static inline def_EHelper(addw) {
  rtl_addw(s,ddest,dsrc1,dsrc2);
  print_asm_template3(addw);
}

static inline def_EHelper(sub) {
  rtl_sub(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sub);
}

static inline def_EHelper(subw) {
  rtl_subw(s,ddest,dsrc1,dsrc2);
  print_asm_template3(subw);
}

//r-type  c_shift_mask 0x3f
static inline def_EHelper(sll) {
  rtl_shl(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sll);
}

//i-type x[rd] = x[rs1] ≪ shamt
static inline def_EHelper(slli) {
  //*s0 = id_src2->imm&0x3f;
  rtl_shli(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(slli);
}

static inline def_EHelper(slliw) {
  //*s0 = id_src2->imm&0x3f;
  rtl_shliw(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(slliw);
}

static inline def_EHelper(sllw) {
  rtl_shlw(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sllw);
}


//x[rd] = (x[rs1] <𝑠 x[rs2])
static inline def_EHelper(slt) {
  rtl_setrelop(s,RELOP_LT,ddest,dsrc1,dsrc2);
  print_asm_template3(slt);
}


static inline def_EHelper(sltu) {
  rtl_setrelop(s,RELOP_LTU,ddest,dsrc1,dsrc2);
  print_asm_template3(sltu);
}

static inline def_EHelper(slti) {
  rtl_setrelopi(s,RELOP_LT,ddest,dsrc1,id_src2->imm);
  print_asm_template3(slti);
}

//x[rd] = (x[rs1] <𝑢 sext(immediate))
static inline def_EHelper(sltiu) {
  //*ddest = (*dsrc1 < id_src2->imm);
  rtl_setrelopi(s,RELOP_LTU,ddest,dsrc1, id_src2->imm);
  print_asm_template3(sltiu);
}

// static inline def_EHelper(sltz) {
//   *s0 = 0;
//   rtl_setrelopi(s,RELOP_LT,ddest,dsrc1,s0);
//   print_asm_template2(sltz);
// }

// static inline def_EHelper(snez) {
//   *s0 = 0;
//   rtl_setrelopi(s,RELOP_NE,ddest,dsrc1,s0);
//   print_asm_template2(snez);
// }


static inline def_EHelper(sra) {
  rtl_sar(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sra);
}

//x[rd] = (x[rs1] ≫𝑠 shamt)  I-type
static inline def_EHelper(srai) {
  //void rtl_sari(DecodeExecState *s, rtlreg_t *dest, const rtlreg_t *src1, const sword_t imm)
  //sar(*src1, imm);
  //src2 imm
  //printf("started srai\n");
  rtl_sari(s, ddest, dsrc1, id_src2->imm);
  //printf("dest value is 0x%lx\n",*ddest);
  print_asm_template3(srai);
}
 

static inline def_EHelper(sraiw) {
  rtl_sariw(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(sraiw);
}

static inline def_EHelper(sraw) {
  rtl_sarw(s,ddest,dsrc1,dsrc2);
  print_asm_template3(sraw);
}


static inline def_EHelper(srl) {
  rtl_shr(s,ddest,dsrc1,dsrc2);
  print_asm_template3(srl);
}

static inline def_EHelper(srli) {
  rtl_shri(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(srli);
}

static inline def_EHelper(srliw) {
  rtl_shriw(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(srliw);
}

static inline def_EHelper(srlw) {
  rtl_shrw(s,ddest,dsrc1,dsrc2);
  print_asm_template3(srlw);
}



static inline def_EHelper(xor) {
  rtl_xor(s,ddest,dsrc1,dsrc2);
  print_asm_template3(xor);
}

static inline def_EHelper(xori) {
  rtl_xori(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(xori);
}

static inline def_EHelper(or) {
  rtl_or(s,ddest,dsrc1,dsrc2);
  print_asm_template3(or);
}

static inline def_EHelper(ori) {
  rtl_ori(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(ori);
}

static inline def_EHelper(and) {
  rtl_and(s,ddest,dsrc1,dsrc2);
  print_asm_template3(and);
}

static inline def_EHelper(andi) {
  rtl_andi(s,ddest,dsrc1,id_src2->imm);
  print_asm_template3(andi);
}

