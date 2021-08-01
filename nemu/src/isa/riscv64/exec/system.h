// I-TYPE : csr(imm), rs1, rd
// ddest, dsrc1, id_src2->simm
// t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
#define CASE_CSR_DO(num,str)                    \
case num: *s0 = cpu.csr.str;           \
          cpu.csr.str = *s0|*dsrc1;    \
          break;

static inline def_EHelper(csrrs) {
  switch (id_src2->imm) {
    // CASE_CSR_DO(0x100,sstatus)
    CASE_CSR_DO(0x105,stvec)
    CASE_CSR_DO(0x141,sepc)
    CASE_CSR_DO(0x142,scause)
    default: break;
  }
  *ddest = *s0;
  print_asm_template2(csrrs);
}

// RaiseException(EnvironmentCall)
static inline def_EHelper(ecall) {
  raise_intr(s, 1, 0);
  print_asm_template1(ecall);
}