#include<monitor/difftest.h>

//x[rd] = pc+4; pc += sext(offset)  J-type
//这里是seq_pc和jmp_pc的用法
static inline def_EHelper(jal) {
  rtl_li(s,ddest,cpu.pc+4);
  //printf("cpu.pc+4 : %ld, seq_pc : %ld\n",cpu.pc+4,s->seq_pc);
  //rtl_li(s,&cpu.pc,cpu.pc+id_src1->imm);
  rtl_j(s,s->jmp_pc);
  //printf("pc : 0x%0lx\n",cpu.pc);
  //printf("ra : 0x%0lx\n",reg_d(1));
  //difftest_skip_dut(1, 2);
  print_asm_template2(jal);
}

//t =pc+8; pc=(x[rs1]+sext(offset))&~1; x[rd]=t ; I-type
static inline def_EHelper(jalr) {
  //printf("Jalr started\n");
  rtl_li(s,s0,cpu.pc+4);
  //printf("cpu.pc+4 : %ld, seq_pc : %ld\n",cpu.pc+4,s->seq_pc);
  rtl_addi(s,s1,dsrc1, id_src2->imm);
  *s1 = (*s1)&(~0x1u);
  rtl_jr(s,s1);
  rtl_li(s,ddest,*s0);
  //difftest_skip_dut(1, 2);
  print_asm_template2(jalr);
}

//if (rs1 == rs2) pc += sext(offset)
static inline def_EHelper(beq) {
  //if(*dsrc1 == *dsrc2) rtl_j(s,s->jmp_pc);
  rtl_jrelop(s,RELOP_EQ, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(beq);
}

//if (rs1 ≠ rs2) pc += sext(offset)
static inline def_EHelper(bne) {
  // if(*dsrc1 != *dsrc2) rtl_j(s,s->jmp_pc);
  //printf("bne started\n");
  //printf("0x%lx, 0x%lx, jmp_pc=0x%lx\n",*dsrc1, *dsrc2,s->jmp_pc);
  rtl_jrelop(s, RELOP_NE, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(bne);
}

static inline def_EHelper(blt) {
  rtl_jrelop(s, RELOP_LT, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(blt);
}

static inline def_EHelper(bltu) {
  rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(bltu);
}

static inline def_EHelper(bge) {
  rtl_jrelop(s, RELOP_GE, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(bge);
}



// static inline def_EHelper(bltu) {
//   rtl_jrelop(s, RELOP_LTU, dsrc1, dsrc2, s->jmp_pc);
//   print_asm_template3(bltu);
// }

static inline def_EHelper(bgeu) {
  rtl_jrelop(s, RELOP_GEU, dsrc1, dsrc2, s->jmp_pc);
  print_asm_template3(bgeu);
}



