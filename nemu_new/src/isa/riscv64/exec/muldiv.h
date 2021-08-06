// static inline make_EHelper(div) {
//   rtl_idiv_q(s,ddest,dsrc1,dsrc2);
//   print_asm_template3(div);
// }

static inline def_EHelper(divu) {
  rtl_div_q(s, ddest, dsrc1, dsrc2);
  print_asm_template3(divu);
}

static inline def_EHelper(divuw) {
  rtl_divuw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(divuw);
}

static inline def_EHelper(divw) {
  rtl_divw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(divw);
}

static inline def_EHelper(mul) {
  rtl_imul_lo(s, ddest, dsrc1, dsrc2);
  print_asm_template3(mul);
}

// static inline make_EHelper(mulh) {
//   rtl_imul_hi(s,ddest,dsrc1,dsrc2);
//   print_asm_template3(mulh);
// }

// static inline make_EHelper(mulhsu) {
//   rtl_mulshu(s,ddest,dsrc1,dsrc2);
//   print_asm_template3(mulhsu);
// }

// static inline make_EHelper(mulhu) {
//   rtl_mulhi(s,ddest,dsrc1,dsrc2);
//   print_asm_template3(mulhu);
// }

static inline def_EHelper(mulw) {
  rtl_mulw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(mulw);
}

static inline def_EHelper(rem) {
  rtl_idiv_r(s, ddest, dsrc1, dsrc2);
  print_asm_template3(rem);
}

static inline def_EHelper(remu) {
  rtl_div_r(s, ddest, dsrc1, dsrc2);
  print_asm_template3(remu);
}

static inline def_EHelper(remw) {
  rtl_remw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(remw);
}

static inline def_EHelper(remuw) {
  rtl_remuw(s, ddest, dsrc1, dsrc2);
  print_asm_template3(remuw);
}
