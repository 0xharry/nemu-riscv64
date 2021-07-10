
// #ifdef _KLIB_TEST_
// #include "../tests/klib_test.h"
// #else
// #include <am.h>
// #include <klib.h>
// #include <klib-macros.h>
// #endif

// #include <stdarg.h>

// #if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
// // formatted output conversion

// /* convert decimal integer to string */
// char* itos_dec(long num, char* str) {
//   if(str == NULL) {
//     // str = (char*) malloc(12); // 最长 int_min 10位十进制整数+负号+终止符号=12位
//     // if(str == NULL)
//     assert(0);
//   }
//   int i = 0;
//   int neg = 1;
//   if(num<0){
//     str[i++] = '-';
//     neg = -1;
//   }

//   long r;
//   do {
//     r = num%10;
//     num /= 10;
//     str[i++] = '0'+neg*r;
//   } while (num);
//   str[i] = '\0';

//   char temp;
//   char* p;
//   if(str[0] == '-') {
//     p = str+1;
//     i--;
//   }
//   else {
//     p = str;
//   }
//   for(int j=0; j<i/2; ++j) {
//     temp = p[j];
//     p[j] = p[i-j-1];
//     p[i-j-1] = temp;
//   }
//   return str;
// }

// char* itos_hex(long num, char* str) {
//   if(str == NULL) {
//     // str = (char*) malloc(12); // 最长 int_min 10位十进制整数+负号+终止符号=12位
//     // if(str == NULL)
//       assert(0);
//   }
//   int i = 0;

//   long r;
//   do {
//     r = num%16;
//     num /= 16;
//     if(r>9) {
//       str[i++] = 'a'+r-10;
//     }
//     else {
//       str[i++] = '0'+r;
//     }
//   } while (num);
//   str[i] = '\0';

//   char temp;
//   char* p = str;
//   for(int j=0; j<i/2; ++j) {
//     temp = p[j];
//     p[j] = p[i-j-1];
//     p[i-j-1] = temp;
//   }
//   return str;
// }

// int vprintf(const char *fmt, va_list p_fmt) {
//   int ret_wordcount=0;
//   char* s = NULL;
//   char num[64];
//   int str_len;
//   int d;
//   long d_long;
//   while(*fmt != '\0') {
//     if(*fmt == '%') {
//       switch (*(++fmt)) {
//         case 'c':
//           d = va_arg(p_fmt, int);
//           putch((char)d);
//           ++ret_wordcount;
//           break;

//         case 's':
//           s = va_arg(p_fmt, char*);
//           str_len = strlen(s);
//           putstr(s);
//           ret_wordcount += str_len;
//           break;

//         case 'd':
//           d=va_arg(p_fmt, int);
//           str_len = strlen(itos_dec(d, num));
//           putstr(num);
//           ret_wordcount += str_len;
//           break;

//         case 'p':
//           d_long=va_arg(p_fmt, int64_t);
//           str_len = strlen(itos_hex(d_long, num));
//           putstr("0x");
//           putstr(num);
//           ret_wordcount += str_len;
//           break;

//         default: break;
//       }
//       fmt++;
//     }
//     else {
//       putch(*fmt++);
//       ++ret_wordcount;
//     }
//   }
//   putch('\0');
//   return ret_wordcount;
// }

// int printf(const char *fmt, ...) {
//   // putch(char ch);
//   va_list p_fmt; 
//   va_start(p_fmt, fmt);
//   int ret = vprintf(fmt, p_fmt);
//   va_end(p_fmt);
//   return ret;
// }

// /* 
//  * The  functions  vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),   vs‐
//  * nprintf()   are   equivalent  to  the  functions  printf(),  fprintf(),
//  * dprintf(), sprintf(), snprintf(), respectively, except  that  they  are
//  * called with a va_list instead of a variable number of arguments.  These
//  * functions do not call the va_end macro.  Because they invoke the va_arg
//  * macro, the value of ap is undefined after the call.
//  */
// int vsprintf(char *out, const char *fmt, va_list p_fmt) {
//   if(out == NULL) return 0;
//   int ret_wordcount=0;
//   char* s = NULL;
//   int str_len;
//   int d;
//   long d_long;
//   while(*fmt != '\0') {
//     if(*fmt == '%') {
//       switch (*(++fmt)) {
//         case 'c':
//           d = va_arg(p_fmt, int);
//           memcpy(out, (char*)&d, 1);
//           ++out;
//           ++ret_wordcount;
//           break;

//         case 's':
//           s = va_arg(p_fmt, char*);
//           str_len = strlen(s);
//           memcpy(out, s, str_len);
//           out += str_len;
//           ret_wordcount += str_len;
//           break;

//         case 'd':
//           d=va_arg(p_fmt, int);
//           str_len = strlen(itos_dec(d, out));
//           out += str_len;
//           ret_wordcount += str_len;
//           break;

//         case 'p':
//           d_long=va_arg(p_fmt, int64_t);
//           *out++ = '0'; *out++ = 'x';
//           str_len = strlen(itos_hex(d_long, out));
//           ret_wordcount += str_len;
//           break;

//         default: break;
//       }
//       fmt++;
//     }
//     else {
//       *out++ = *fmt++;
//       ++ret_wordcount;
//     }
//   }
//   *out = '\0';
//   return ret_wordcount;
// }

// // sprintf()  write output  to  the  given  output  stream
// int sprintf(char *out, const char *fmt, ...) {
//   va_list p_fmt; 
//   va_start(p_fmt, fmt);
//   int ret = vsprintf(out, fmt, p_fmt);
//   va_end(p_fmt);
//   return ret;
// }

// // int tail()

// int vsnprintf(char *out, size_t n, const char *fmt, va_list p_fmt) {
//   if(out == NULL || n-- <=0) return 0;
//   int ret_wordcount=0;
//   char* s = NULL;
//   int str_len;
//   int d;
//   long d_long;
//   char d_buf[12];
//   while(*fmt != '\0') {
//     if(*fmt == '%') {
//       switch (*(++fmt)) {
//         case 'c':
//           d = va_arg(p_fmt, int);
//           memcpy(out, (char*)&d, 1);
//           ++out;
//           ++ret_wordcount;
//           break;

//         case 's':
//           s = va_arg(p_fmt, char*);
//           str_len = strlen(s);
//           if(ret_wordcount + str_len > n) {
//             d = n - ret_wordcount;
//             memcpy(out, s, d);
//             *(out+d+1) = '\0';
//             return n;
//           }
//           memcpy(out, s, str_len);
//           out += str_len;
//           ret_wordcount += str_len;
//           break;

//         case 'd':
//           d=va_arg(p_fmt, int);
//           str_len = strlen(itos_dec(d, d_buf));
//           if(ret_wordcount + str_len > n) {
//             d = n - ret_wordcount;
//             memcpy(out, d_buf, d);
//             *(out+d+1) = '\0';
//             return n;
//           }
//           memcpy(out, d_buf, str_len);
//           out += str_len;
//           ret_wordcount += str_len;
//           break;

//         case 'p':
//           d_long=va_arg(p_fmt, int64_t);
//           // *out++ = '0'; *out++ = 'x';
//           str_len = strlen(itos_hex(d_long, d_buf));
//           if(ret_wordcount + str_len + 2 > n) {
//             d = n - ret_wordcount;
//             if(d<3) return n;
//             memcpy(out, d_buf, d);
//             *(out+d+1) = '\0';
//             return n;
//           }
//           memcpy(out, d_buf, str_len);
//           out += str_len;
//           ret_wordcount += str_len;
//           break;

//         default: break;
//       }
//       fmt++;
//     }
//     else {
//       *out++ = *fmt++;
//       ++ret_wordcount;
//     }
//     if(ret_wordcount >= n) {
//       *out = '\0';
//       return ret_wordcount;
//     }
//   }
//   *out = '\0';
//   return ret_wordcount;
// }

// /* snprintf
//  * The  functions snprintf() and vsnprintf() write at most n bytes (in‐
//  * cluding the terminating null byte ('\0')) to out.
//  */
// int snprintf(char *out, size_t n, const char *fmt, ...) {
//   va_list p_fmt; 
//   va_start(p_fmt, fmt);
//   int ret = vsnprintf(out, n, fmt, p_fmt);
//   va_end(p_fmt);
//   return ret;
// }

// #endif
#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

/*
 * The functions in the printf() family produce output according to a for‐
 * mat  as  described  below.   The functions printf() and vprintf() write
 * output to stdout, the standard output stream; fprintf() and  vfprintf()
 * write  output  to  the  given  output  stream;  sprintf(),  snprintf(),
 * vsprintf() and vsnprintf() write to the character string str.
 * 
 * The function dprintf() is the same as fprintf() except that it  outputs
 * to a file descriptor, fd, instead of to a stdio stream.
 * 
 * The  functions snprintf() and vsnprintf() write at most size bytes (in‐
 * cluding the terminating null byte ('\0')) to str.
 *
 * The  functions  vprintf(),  vfprintf(),  vdprintf(),  vsprintf(),   vs‐
 * nprintf()   are   equivalent  to  the  functions  printf(),  fprintf(),
 * dprintf(), sprintf(), snprintf(), respectively, except  that  they  are
 * called with a va_list instead of a variable number of arguments.  These
 * functions do not call the va_end macro.  Because they invoke the va_arg
 * macro, the value of ap is undefined after the call.  See stdarg(3).
 *
 * All  of  these functions write the output under the control of a format
 * string that specifies how subsequent arguments (or  arguments  accessed
 * via the variable-length argument facilities of stdarg(3)) are converted
 * for output.
 */

/* 
 * snprintf() and vsnprintf() write at most size bytes (in‐
 * cluding the terminating null byte ('\0')) to str.
 */
/**
 * Very portable snprintf implementation, limited in functionality,
 * esp. for %[capital] %[nonportable] and so on.  Reduced float functionality,
 * mostly in formatting and range (e+-16), for %f and %g.
 *
 * %s, %d, %u, %i, %x, %c, %n and %% are fully supported.
 *   This includes width, precision, flags 0- +, and *(arg for wid,prec).
 * %f, %g, %m, %p have reduced support, support for wid,prec,flags,*, but
 *   less floating point range, no %e formatting for %g.
 */

/** add padding to string */
static void
print_pad(char** at, size_t* left, int* ret, char p, int num)
{
	while(num--) {
		if(*left > 1) {
			*(*at)++ = p;
			(*left)--;
		}
		(*ret)++;
	}
}

/** get negative symbol, 0 if none */
static char
get_negsign(int negative, int plus, int space)
{
	if(negative)
		return '-';
	if(plus)
		return '+';
	if(space)
		return ' ';
	return 0;
}

#define PRINT_DEC_BUFSZ 32 /* 20 is enough for 64 bit decimals */
/** print decimal into buffer, returns length */
static int
print_dec(char* buf, int max, unsigned int value)
{
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = '0' + value % 10;
		value /= 10;
	}
	return i;
}

/** print long decimal into buffer, returns length */
static int
print_dec_l(char* buf, int max, unsigned long value)
{
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = '0' + value % 10;
		value /= 10;
	}
	return i;
}

/** print long decimal into buffer, returns length */
static int
print_dec_ll(char* buf, int max, unsigned long long value)
{
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = '0' + value % 10;
		value /= 10;
	}
	return i;
}

/** print hex into buffer, returns length */
static int
print_hex(char* buf, int max, unsigned int value)
{
	const char* h = "0123456789abcdef";
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = h[value & 0x0f];
		value >>= 4;
	}
	return i;
}

/** print long hex into buffer, returns length */
static int
print_hex_l(char* buf, int max, unsigned long value)
{
	const char* h = "0123456789abcdef";
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = h[value & 0x0f];
		value >>= 4;
	}
	return i;
}

/** print long long hex into buffer, returns length */
static int
print_hex_ll(char* buf, int max, unsigned long long value)
{
	const char* h = "0123456789abcdef";
	int i = 0;
	if(value == 0) {
		if(max > 0) {
			buf[0] = '0';
			i = 1;
		}
	} else while(value && i < max) {
		buf[i++] = h[value & 0x0f];
		value >>= 4;
	}
	return i;
}

/** copy string into result, reversed */
static void
spool_str_rev(char** at, size_t* left, int* ret, const char* buf, int len)
{
	int i = len;
	while(i) {
		if(*left > 1) {
			*(*at)++ = buf[--i];
			(*left)--;
		} else --i;
		(*ret)++;
	}
}

/** copy string into result */
static void
spool_str(char** at, size_t* left, int* ret, const char* buf, int len)
{
	int i;
	for(i=0; i<len; i++) {
		if(*left > 1) {
			*(*at)++ = buf[i];
			(*left)--;
		}
		(*ret)++;
	}
}

/** print number formatted */
static void
print_num(char** at, size_t* left, int* ret, int minw, int precision,
	int prgiven, int zeropad, int minus, int plus, int space,
	int zero, int negative, char* buf, int len)
{
	int w = len; /* excludes minus sign */
	char s = get_negsign(negative, plus, space);
	if(minus) {
		/* left adjust the number into the field, space padding */
		/* calc numw = [sign][zeroes][number] */
		int numw = w;
		if(precision == 0 && zero) numw = 0;
		if(numw < precision) numw = precision;
		if(s) numw++;

		/* sign */
		if(s) print_pad(at, left, ret, s, 1);

		/* number */
		if(precision == 0 && zero) {
			/* "" for the number */
		} else {
			if(w < precision)
				print_pad(at, left, ret, '0', precision - w);
			spool_str_rev(at, left, ret, buf, len);
		}
		/* spaces */
		if(numw < minw)
			print_pad(at, left, ret, ' ', minw - numw);
	} else {
		/* pad on the left of the number */
		/* calculate numw has width of [sign][zeroes][number] */
		int numw = w;
		if(precision == 0 && zero) numw = 0;
		if(numw < precision) numw = precision;
		if(!prgiven && zeropad && numw < minw) numw = minw;
		else if(s) numw++;

		/* pad with spaces */
		if(numw < minw)
			print_pad(at, left, ret, ' ', minw - numw);
		/* print sign (and one less zeropad if so) */
		if(s) {
			print_pad(at, left, ret, s, 1);
			numw--;
		}
		/* pad with zeroes */
		if(w < numw)
			print_pad(at, left, ret, '0', numw - w);
		if(precision == 0 && zero)
			return;
		/* print the characters for the value */
		spool_str_rev(at, left, ret, buf, len);
	}
}

/** print %d and %i */
static void
print_num_d(char** at, size_t* left, int* ret, int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = (value < 0);
	int zero = (value == 0);
	int len = print_dec(buf, (int)sizeof(buf),
		(unsigned int)(negative?-value:value));
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %ld and %li */
static void
print_num_ld(char** at, size_t* left, int* ret, long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = (value < 0);
	int zero = (value == 0);
	int len = print_dec_l(buf, (int)sizeof(buf),
		(unsigned long)(negative?-value:value));
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %lld and %lli */
static void
print_num_lld(char** at, size_t* left, int* ret, long long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = (value < 0);
	int zero = (value == 0);
	int len = print_dec_ll(buf, (int)sizeof(buf),
		(unsigned long long)(negative?-value:value));
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %u */
static void
print_num_u(char** at, size_t* left, int* ret, unsigned int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_dec(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %lu */
static void
print_num_lu(char** at, size_t* left, int* ret, unsigned long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_dec_l(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %llu */
static void
print_num_llu(char** at, size_t* left, int* ret, unsigned long long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_dec_ll(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %x */
static void
print_num_x(char** at, size_t* left, int* ret, unsigned int value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_hex(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %lx */
static void
print_num_lx(char** at, size_t* left, int* ret, unsigned long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_hex_l(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %llx */
static void
print_num_llx(char** at, size_t* left, int* ret, unsigned long long value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	int len = print_hex_ll(buf, (int)sizeof(buf), value);
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}

/** print %llp */
static void
print_num_llp(char** at, size_t* left, int* ret, void* value,
	int minw, int precision, int prgiven, int zeropad, int minus,
	int plus, int space)
{
	char buf[PRINT_DEC_BUFSZ];
	int negative = 0;
	int zero = (value == 0);
	unsigned long long llvalue = (unsigned long long)value;
	int len = print_hex_ll(buf, (int)sizeof(buf), llvalue);
	if(zero) {
		buf[0]=')';
		buf[1]='l';
		buf[2]='i';
		buf[3]='n';
		buf[4]='(';
		len = 5;
	} else {
		/* put '0x' in front of the (reversed) buffer result */
		if(len < PRINT_DEC_BUFSZ)
			buf[len++] = 'x';
		if(len < PRINT_DEC_BUFSZ)
			buf[len++] = '0';
	}
	print_num(at, left, ret, minw, precision, prgiven, zeropad, minus,
		plus, space, zero, negative, buf, len);
}


/** strnlen (compat implementation) */
static int
my_strnlen(const char* s, int max)
{
	int i;
	for(i=0; i<max; i++)
		if(s[i]==0)
			return i;
	return max;
}

/** print %s */
static void
print_str(char** at, size_t* left, int* ret, char* s,
	int minw, int precision, int prgiven, int minus)
{
	int w;
	/* with prec: no more than x characters from this string, stop at 0 */
	if(prgiven)
		w = my_strnlen(s, precision);
	else	w = (int)strlen(s); /* up to the nul */
	if(w < minw && !minus)
		print_pad(at, left, ret, ' ', minw - w);
	spool_str(at, left, ret, s, w);
	if(w < minw && minus)
		print_pad(at, left, ret, ' ', minw - w);
}

/** print %c */
static void
print_char(char** at, size_t* left, int* ret, int c,
	int minw, int minus)
{
	if(1 < minw && !minus)
		print_pad(at, left, ret, ' ', minw - 1);
	print_pad(at, left, ret, c, 1);
	if(1 < minw && minus)
		print_pad(at, left, ret, ' ', minw - 1);
}


/** 
 * Print to string.
 * str: string buffer for result. result will be null terminated.
 * size: size of the buffer. null is put inside buffer.
 * format: printf format string.
 * arg: '...' arguments to print.
 * returns number of characters. a null is printed after this.
 * return number of bytes that would have been written
 *	   if the buffer had been large enough.
 * 
 * supported format specifiers:
 * 	%s, %u, %d, %x, %i, %f, %g, %c, %p, %n.
 * 	length: l, ll (for d, u, x).
 * 	precision: 6.6d (for d, u, x)
 * 		%f, %g precisions, 0.3f
 * 		%20s, '.*s'
 * 	and %%.
 */
int vsnprintf(char* str, size_t size, const char* format, va_list arg)
{
	char* at = str;
	size_t left = size;
	int ret = 0;
	const char* fmt = format;
	int conv, minw, precision, prgiven, zeropad, minus, plus, space, length;
	while(*fmt) {
		/* copy string before % */
		while(*fmt && *fmt!='%') {
			if(left > 1) {
				*at++ = *fmt++;
				left--;
			} else fmt++;
			ret++;
		}
		
		/* see if we are at end */
		if(!*fmt) break;

		/* fetch next argument % designation from format string */
		fmt++; /* skip the '%' */

		/********************************/
		/* get the argument designation */
		/********************************/
		/* we must do this vararg stuff inside this function for
		 * portability.  Hence, get_designation, and print_designation
		 * are not their own functions. */

		/* printout designation:
		 * conversion specifier: x, d, u, s, c, n, m, p
		 * flags: # not supported
		 *        0 zeropad (on the left)
		 *	  - left adjust (right by default)
		 *	  ' ' printspace for positive number (in - position).
		 *	  + alwayssign
		 * fieldwidth: [1-9][0-9]* minimum field width.
		 * 	if this is * then type int next argument specifies the minwidth.
		 * 	if this is negative, the - flag is set (with positive width).
		 * precision: period[digits]*, %.2x.
		 * 	if this is * then type int next argument specifies the precision.
		 *	just '.' or negative value means precision=0.
		 *		this is mindigits to print for d, i, u, x
		 *		this is aftercomma digits for f
		 *		this is max number significant digits for g
		 *		maxnumber characters to be printed for s
		 * length: 0-none (int), 1-l (long), 2-ll (long long)
		 * 	notsupported: hh (char), h (short), L (long double), q, j, z, t
		 * Does not support %m$ and *m$ argument designation as array indices.
		 * Does not support %#x
		 *
		 */
		minw = 0;
		precision = 1;
		prgiven = 0;
		zeropad = 0;
		minus = 0;
		plus = 0;
		space = 0;
		length = 0;

		/* get flags in any order */
		for(;;) {
			if(*fmt == '0')
				zeropad = 1;
			else if(*fmt == '-')
				minus = 1;
			else if(*fmt == '+')
				plus = 1;
			else if(*fmt == ' ')
				space = 1;
			else break;
			fmt++;
		}

		/* field width */
		if(*fmt == '*') {
			fmt++; /* skip char */
			minw = va_arg(arg, int);
			if(minw < 0) {
				minus = 1;
				minw = -minw;
			}
		} else while(*fmt >= '0' && *fmt <= '9') {
			minw = minw*10 + (*fmt++)-'0';
		}

		/* precision */
		if(*fmt == '.') {
			fmt++; /* skip period */
			prgiven = 1;
			precision = 0;
			if(*fmt == '*') {
				fmt++; /* skip char */
				precision = va_arg(arg, int);
				if(precision < 0)
					precision = 0;
			} else while(*fmt >= '0' && *fmt <= '9') {
				precision = precision*10 + (*fmt++)-'0';
			}
		}

		/* length */
		if(*fmt == 'l') {
			fmt++; /* skip char */
			length = 1;
			if(*fmt == 'l') {
				fmt++; /* skip char */
				length = 2;
			}
		}

		/* get the conversion */
		if(!*fmt) conv = 0;
		else	conv = *fmt++;

		/***********************************/
		/* print that argument designation */
		/***********************************/
		switch(conv) {
		case 'i':
		case 'd':
			if(length == 0)
			    print_num_d(&at, &left, &ret, va_arg(arg, int),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 1)
			    print_num_ld(&at, &left, &ret, va_arg(arg, long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 2)
			    print_num_lld(&at, &left, &ret,
				va_arg(arg, long long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			break;
		case 'u':
			if(length == 0)
			    print_num_u(&at, &left, &ret,
				va_arg(arg, unsigned int),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 1)
			    print_num_lu(&at, &left, &ret,
				va_arg(arg, unsigned long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 2)
			    print_num_llu(&at, &left, &ret,
				va_arg(arg, unsigned long long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			break;
		case 'x':
			if(length == 0)
			    print_num_x(&at, &left, &ret,
				va_arg(arg, unsigned int),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 1)
			    print_num_lx(&at, &left, &ret,
				va_arg(arg, unsigned long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			else if(length == 2)
			    print_num_llx(&at, &left, &ret,
				va_arg(arg, unsigned long long),
				minw, precision, prgiven, zeropad, minus, plus, space);
			break;
		case 's':
			print_str(&at, &left, &ret, va_arg(arg, char*),
				minw, precision, prgiven, minus);
			break;
		case 'c':
			print_char(&at, &left, &ret, va_arg(arg, int),
				minw, minus);
			break;
		case 'n':
			*va_arg(arg, int*) = ret;
			break;
		case 'p':
			print_num_llp(&at, &left, &ret, va_arg(arg, void*),
				minw, precision, prgiven, zeropad, minus, plus, space);
			break;
		case '%':
			print_pad(&at, &left, &ret, '%', 1);
			break;
		/* unknown */
		default:
		case 0: break;
		}
	}

	/* zero terminate */
	if(left > 0)
		*at = 0;
	return ret;
}

int snprintf(char* str, size_t size, const char* format, ...) {
	va_list args;
	va_start(args, format);
	int ret = vsnprintf(str, size, format, args);
	va_end(args);
	return ret;
}


/* -------------------------------------printf------------------------------------------
 * print "format" to str
 */
int vsprintf(char *str, const char *format, va_list ap) {
	return 0; //vsnprintf(str, -1, format, ap);
}

int sprintf(char *str, const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int ret = vsprintf(str, format, ap);
  va_end(ap);
  return ret;
}

char ostream[4096];
int printf(const char *format, ...) {
  va_list ap;
  va_start(ap, format);
  int ret = vsprintf(ostream, format, ap);
  va_end(ap);
  putstr(ostream);
  return ret;
}

#endif