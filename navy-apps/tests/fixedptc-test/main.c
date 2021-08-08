#include <stdio.h>
#include <assert.h>
#include <fixedptc.h>

int main() {
  fixedpt a = fixedpt_rconst(-1.5);
  fixedpt b = fixedpt_rconst(1.5);
  fixedpt c = fixedpt_rconst(5.5*0.2);
  fixedpt d = fixedpt_rconst(5.6/0.2);
  fixedpt e = fixedpt_rconst(65535);
  printf("a = %d   b = %d  c = %d  d = %d e = %d\n",a,b,c,d,e);
  printf("res a  floor = %d  ceil = %d\n",fixedpt_floor(a),fixedpt_ceil(a));
  printf("res b  floor = %d  ceil = %d\n",fixedpt_floor(b),fixedpt_ceil(b));
  printf("res c  floor = %d  ceil = %d\n",fixedpt_floor(c),fixedpt_ceil(c));
  printf("res d  floor = %d  ceil = %d\n",fixedpt_floor(d),fixedpt_ceil(d));
  printf("res e  floor = %d  ceil = %d\n",fixedpt_floor(e),fixedpt_ceil(e));
  assert(fixedpt_ceil(a)-fixedpt_floor(a) == 256);
  assert(fixedpt_ceil(b)-fixedpt_floor(b) == 256);
  assert(fixedpt_ceil(c)-fixedpt_floor(c) == 256);
  assert(fixedpt_ceil(d)-fixedpt_floor(d) == 0);
  return 0;
}
