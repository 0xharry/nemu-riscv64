// I-TYPE : csr(imm), rs1, rd
// t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
static inline def_EHelper(csrrs)
{
  TODO();
  print_asm_template2(csrrs);
}

// RaiseException(EnvironmentCall)
static inline def_EHelper(ecall)
{
  // raise_intr(s, 1, );
  print_asm_template1(ecall);
}