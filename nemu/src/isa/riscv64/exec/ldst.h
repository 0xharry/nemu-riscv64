static inline def_EHelper(ld) {
  rtl_lms(s, ddest, dsrc1, id_src2->imm, s->width); // id_src2->imm

  print_Dop(id_src1->str, OP_STR_SIZE, "%ld(%s)", id_src2->imm, reg_name(id_src1->reg));
  switch (s->width) {
    case 4: print_asm_template2(lw); break;
    case 2: print_asm_template2(lh); break;
    case 1: print_asm_template2(lb); break;
    default: assert(0);
  }
}

static inline def_EHelper(ldu) {
  rtl_lm(s, ddest, dsrc1, id_src2->imm, s->width);

  print_Dop(id_src1->str, OP_STR_SIZE, "%ld(%s)", id_src2->imm, reg_name(id_src1->reg));
  switch (s->width) {
    case 8: print_asm_template2(ld);  break;
    case 4: print_asm_template2(lwu); break;
    case 2: print_asm_template2(lhu); break;
    case 1: print_asm_template2(lbu); break;
    default: assert(0);
  }
}

static inline def_EHelper(st) {
  rtl_sm(s, dsrc1, id_src2->imm, ddest, s->width); // id_src2->imm

  print_Dop(id_src1->str, OP_STR_SIZE, "%ld(%s)", id_src2->imm, reg_name(id_src1->reg));
  switch (s->width) {
    case 8: print_asm_template2(sd); break;
    case 4: print_asm_template2(sw); break;
    case 2: print_asm_template2(sh); break;
    case 1: print_asm_template2(sb); break;
    default: assert(0);
  }
}
