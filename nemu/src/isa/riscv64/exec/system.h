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

#ifdef DIFF_TEST
extern void difftest_skip_dut(int nr_ref, int nr_dut);
#endif
// RaiseException(EnvironmentCall)
static inline def_EHelper(ecall) {
  cpu.csr.sstatus.val=0xc0100;
  raise_intr(s, 9, 0);
  print_asm_template1(ecall);
#ifdef DIFF_TEST
  difftest_skip_dut(1, 2);
#endif
}

extern bool sret_jump;
// ExceptionReturn(Supervisor)
static inline def_EHelper(sret) {
#ifdef DEBUG
  Log("sret to %p", (void*)cpu.csr.sepc);
#endif
  rtl_j(s, cpu.csr.sepc+4);
  // sret_jump = 1;
  print_asm_template1(sret);
}