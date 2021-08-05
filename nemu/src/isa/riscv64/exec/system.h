
//rtl_li  *dst =  imm
//rtl_mv  *dst = *src

//返回值与s0 s1一致
static inline rtlreg_t* encode_csr(uint32_t csr) {
  rtlreg_t *ret = (void*)0;
  switch(csr) {
    //supervisor trap setup
    case 0x100: ret=(rtlreg_t *)&cpu.sstatus.val;break;
    //case 0x102: ret=&cpu.sedeleg    ;break;  
    //case 0x103: ret=&cpu.sideleg    ;break;
    //case 0x104: ret=&cpu.sstatus.sie        ;break;
    case 0x105: ret=(rtlreg_t *)&cpu.stvec      ;break;
    //case 0x106: ret=&cpu.scounteren;break;

    //supervisor trap setup
    case 0x140: ret=(rtlreg_t *)&cpu.sscratch   ;break;
    case 0x141: ret=(rtlreg_t *)&cpu.sepc       ;break;
    case 0x142: ret=(rtlreg_t *)&cpu.scause     ;break;
    case 0x143: ret=(rtlreg_t *)&cpu.stval      ;break;
    // case 0x144: ret=&cpu.sip        ;break;
    // case 0x180: ret=&cpu.satp       ;break;

    default: Log("wrong csr code 0x%x\n",csr);
  }
  return ret;
}


extern void raise_intr(DecodeExecState *s, word_t NO, vaddr_t epc);

//ecall sret
static inline def_EHelper(inter) {
  switch(s->isa.instr.csr.csr) {
    case 0x0: 
      raise_intr(s, 9, cpu.pc);
      print_asm("ecall");
      break;
    case 0x102:
      //置 pc 为 CSRs[sepc]，权限模式为 CSRs[sstatus].SPP，
      //CSRs[sstatus].SIE 为 CSRs[sstatus].SPIE，CSRs[sstatus].SPIE 为 1，CSRs[sstatus].spp 为 0。
      //cpu.pc = cpu.sepc;
      cpu.sstatus.sie = cpu.sstatus.spie;
      cpu.sstatus.spie = 1;
      //printf("in intet sret cpu.sepc : 0x%lx\n",cpu.sepc);
      rtl_j(s, cpu.sepc);
      //TODO():这里忽略了spp权限模式位
      print_asm("sret");
      break;
    default: Log("wrong csr code 0x%x\n",s->isa.instr.csr.csr);
  }
}

//t = CSRs[csr]; CSRs[csr] = x[rs1]; x[rd] = t
static inline def_EHelper(csrrw) {
  //t = CSRs[csr]
  rtlreg_t* t = encode_csr(id_src2->imm);
  rtlreg_t* t_tmp = t;
  rtl_mv(s,t,dsrc1);
  rtl_mv(s,ddest,t_tmp);
  print_asm_template3(csrrw);
}



//t = CSRs[csr]; CSRs[csr] = t | x[rs1]; x[rd] = t
static inline def_EHelper(csrrs) {
  // printf("get in csrrs\n");
  // printf("ddst->val : 0x%lx , dsrc->val : 0x%lx\n",*ddest,*dsrc1);
  rtlreg_t* t = encode_csr(id_src2->imm);
  rtlreg_t* t_tmp = t;
  rtl_or(s,t,t,dsrc1);
  rtl_mv(s,ddest,t_tmp);
  print_asm_template3(csrrs);
}

//t = CSRs[csr]; CSRs[csr] = t &~x[rs1]; x[rd] = t
static inline def_EHelper(csrrc) {
  rtlreg_t* t = encode_csr(id_src2->imm);
  rtlreg_t* t_tmp = t;
  rtl_not(s,s0,dsrc1);
  rtl_and(s,t,t,s0);
  rtl_mv(s,ddest,t_tmp);
  print_asm_template3(csrrc);
}

//x[rd] = CSRs[csr]; CSRs[csr] = zimm
static inline def_EHelper(csrrwi) {
  rtlreg_t* t = encode_csr(id_src2->imm);
  rtl_mv(s,ddest,t);
  rtl_li(s,t,id_src1->imm);
  print_asm_template3(csrrwi);
}

//CSRs[csr] |= zimm
static inline def_EHelper(csrrci) {
  rtlreg_t* t = encode_csr(id_src2->imm);
  rtl_or(s,t,t,dsrc1);
  print_asm_template3(csrrci);
}


static inline def_EHelper(csrrsi) {
  TODO();
  //print_asm_template3(csrrsi);
}
