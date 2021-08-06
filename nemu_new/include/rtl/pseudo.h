#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */
// *dest = imm
static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

//*dest = *src1
static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz);
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1
  *dest = ~(*src1);
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- -src1
  *dest = -1*(*src1);
}

//符号位拓展
static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 8 - 1) .. 0])
    switch (width) {
    case 8: *dest = (rtlreg_t)(*src1); break;
    case 4: *dest = (rtlreg_t)(int64_t)(int32_t)(*src1); break;
    case 2: *dest = (rtlreg_t)(int64_t)(int16_t)(*src1); break;
    case 1: *dest = (*src1)==0?(rtlreg_t)0:(rtlreg_t)(0xffffffff); break;
    default: printf("error width %d\n",width);
  }
}

//0拓展
static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
    switch (width) {
    case 8: *dest = (rtlreg_t)(*src1); break;
    case 4: *dest = (rtlreg_t)(uint64_t)(uint32_t)(*src1); break;
    case 2: *dest = (rtlreg_t)(uint64_t)(uint16_t)(*src1); break;
    case 1: *dest = (*src1)==0?(rtlreg_t)0:(rtlreg_t)(0x00000001); break;
    default: assert(0);
  }
}

//获取符号位
static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
    int len = width*8-1;
    *dest = ((*src1)>>len)&0x1;
}

#endif
