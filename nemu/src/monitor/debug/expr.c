#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  // 一些基本运算符号
  TK_MINUS, // -减号
  // TK_NEG,   // -负号
  TK_PLUS,  // +
  TK_MUL,   // *
  TK_DIV,   // /
  // TK_ZERO,  // 0可能用上
  // // 二进制运算
  // AND, // &
  // OR,  // |
  // XOR, // ^
  // SHL, // <<
  // SHR, // 不支持>>为好 

  // 比较
  TK_EQ,  // ==
  // TK_NEQ, // !=
  // TK_L,   // <
  // TK_LE,  // <=
  // TK_G,   // >
  // TK_GE,  // >=

  // 括号
  TK_LP, // '(' left parenthese
  TK_RP, // ')' right parenthese

  // 操作数
  TK_IDEC, // Decimal integer
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\d", TK_IDEC}, // Decimal integer
  {"\\+", TK_PLUS},     // plus
  {"\\(", TK_LP},  // left parenthesis
  {"\\)", TK_RP},  // right parenthesis
  {"\\*", TK_MUL}, // multiply
  {"-", TK_MINUS}, // minus
  {"\\/", TK_DIV}, // divide
  {"==", TK_EQ},        // equal
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_},
  // {"", TK_}  
};

// 规则数量
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

// 编译后的特定re格式
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
    else
      printf("add rule %d\n",rules[i].token_type);
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

// 32个token是否不够用？
static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  // 按顺序挨个字符（串）匹配re规则
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      // 一条规则匹配成功, 并且匹配出的子串正好是position所在位置
      bool ret = regexec(&re[i], e + position, 1, &pmatch, 0);
      if ( ret == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_IDEC:
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].type = TK_IDEC;
            break;

          case TK_PLUS:
          case TK_LP:
          case TK_RP:
          case TK_MUL:
          case TK_MINUS:
          case TK_DIV:
          case TK_EQ:
            tokens[nr_token++].type = rules[i].token_type;
          case TK_NOTYPE:
            break;

          default: Log("no match rules like \"%d\" type\n at position %d with len %d: %.*s"\
          , rules[i].token_type ,position, substr_len, substr_len, substr_start);
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();

  return 0;
}
