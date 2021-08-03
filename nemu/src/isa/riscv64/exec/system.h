// I-TYPE : csr(imm), rs1, rd
// ddest, dsrc1, id_src2->simm
// t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
#define CASE_CSR_DO_s(num,str)         \
case num: *s0 = cpu.csr.str;           \
          cpu.csr.str = *s0|*dsrc1;    \
          break;

#define CASE_CSR_DO_w(num,str)         \
case num: *ddest = cpu.csr.str;        \
          cpu.csr.str = *dsrc1;        \
          break;

static inline def_EHelper(csrrs) {
  switch (id_src2->imm) {
    CASE_CSR_DO_s(0x100,sstatus.val)
    CASE_CSR_DO_s(0x105,stvec)
    CASE_CSR_DO_s(0x141,sepc)
    CASE_CSR_DO_s(0x142,scause)
    default: break;
  }
  *ddest = *s0;
  print_asm_template3(csrrs);
}

static inline def_EHelper(csrrw) {
  switch (id_src2->imm) {
    CASE_CSR_DO_w(0x100,sstatus.val)
    CASE_CSR_DO_w(0x105,stvec)
    CASE_CSR_DO_w(0x141,sepc)
    CASE_CSR_DO_w(0x142,scause)
    default: break;
  }
  print_asm_template3(csrrs);
}

// RaiseException(EnvironmentCall)
static inline def_EHelper(ecall) {
  raise_intr(s, 1, 0);
// // back to next instruction
//   s->is_jmp = false;
//   cpu.pc = cpu.csr.sepc;
  print_asm_template1(ecall);
}

// ExceptionReturn(Supervisor)
static inline def_EHelper(sret) {
  assert(s->is_jmp == false);
  cpu.pc = cpu.csr.sepc;
  print_asm_template1(sret);
}