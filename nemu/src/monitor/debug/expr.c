#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256,
  // 一些基本运算符号
  TK_MINUS, // -减号
  
  TK_PLUS,  // +

  TK_MUL,   // *
  TK_DIV,   // /
  // TK_ZERO,  // 0可能用上?
  // // 二进制运算
  // AND, // &
  // OR,  // |
  // XOR, // ^
  // SHL, // <<
  // SHR, // >> 若实现，算数？逻辑？ 

  // 比较
  TK_EQ,  // ==
  TK_NEQ, // !=
  TK_AND, // &&
  TK_L,   // <
  TK_LE,  // <=
  TK_G,   // >
  TK_GE,  // >=

  // 括号
  TK_LP, // '(' left parenthese
  TK_RP, // ')' right parenthese

  // 操作数
  TK_DEC, // Decimal integer
  TK_HEX, // 0x Hexadecimal integer
  TK_REG, // $  Register

  // // 没必要
  // TK_DEREF, // * dereference
  // TK_NEG,   // -负号
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
  {"\\+", TK_PLUS},     // plus
  {"\\(", TK_LP},       // left parenthesis
  {"\\)", TK_RP},       // right parenthesis
  {"\\*", TK_MUL},      // multiply
  {"-", TK_MINUS},      // minus
  {"\\/", TK_DIV},      // divide
  {"==", TK_EQ},        // equal
  {"[0-9]+", TK_DEC},   // Decimal integer
  {"0x[0-9]+", TK_HEX}, // Hexadecimal integer
  {"$..", TK_REG},  // Register
  {"!=", TK_NEQ},   // not equal
  {"&&", TK_AND},   // and
  {"<", TK_L},  // less
  {"<=", TK_LE},  // less or equal
  {">", TK_G},  // great
  {">=", TK_GE},  // great or equal
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
          case TK_DEC:
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].type = TK_DEC;
            break;

          case TK_HEX:
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token++].type = TK_DEC;
            break;

          case TK_REG:
            strncpy(tokens[nr_token].str, substr_start+1, substr_len-1);
            tokens[nr_token++].type = TK_DEC;
            break;

          case TK_PLUS:
          case TK_LP:
          case TK_RP:
          case TK_MUL:
          case TK_MINUS:
          case TK_DIV:
          case TK_EQ:
          case TK_NEQ:
          case TK_AND:
          case TK_L:
          case TK_LE:
          case TK_G:
          case TK_GE:
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

bool check_parentheses(int p, int q)
{
  if(tokens[p].type != TK_LP\
  || tokens[q].type != TK_RP)
    return false;
  p++;
  q--;
  // 双指针，相遇时退出，判断左右括号是否>0且相等
  int left=1, right=1;
  while(p <= q)
  {
    // 左指针
    switch (tokens[p++].type)
    {
      case TK_LP: left++; break;
      case TK_RP: left--; break;
      default: break;
    }
    if(p>q) break;

    // 右指针
    switch (tokens[q--].type)
    {
      case TK_RP: right++; break;
      case TK_LP: right--; break;
      default: break;
    }
  }
  if(left>0 && left == right)
    return true;
  else
    return false;
}

#include <limits.h>
int find_main_op(int p, int q)
{
  int ret_op = INT_MAX, curr_op_type = INT_MAX;
  int level=0, curr_op_level = INT_MAX;
  while (p <= q)
  {
    switch (tokens[p++].type)
    {
      // 左括号可提升op优先级
      case TK_LP: level++; break;
      // 右括号降低接下来op优先级
      case TK_RP: level--;
        Assert(level>=0, "right parenthesis cant be negative");
        break;

      case TK_PLUS:
      case TK_MINUS:
        // 如果已经有更低/平级level的加减op被登记，跳过
        if((curr_op_level < level) ||\
          ((curr_op_level == level) && (curr_op_type <= TK_PLUS)))
          break;
        curr_op_level = level;
        curr_op_type = tokens[p-1].type;
        ret_op = p-1;
        break;

      case TK_MUL:
      case TK_DIV:
        // 如果已经有更低/平级level的op被登记，跳过
        if(curr_op_level <= level)
          break;
        curr_op_level = level;
        curr_op_type = tokens[p-1].type;
        ret_op = p-1;
        break;
        
      default:
        // 非op tokens
        break;
    }
  }
  
  if(ret_op == INT_MAX)
  {
    // 未匹配到任何op
    return -1;
  }
  return ret_op;
}

word_t eval(int p, int q) {
  if (p > q) {
    Assert(0,"p > q, Bad expression"); /* Bad expression */
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    word_t ret;
    switch (tokens[p].type)
    {
      case TK_DEC:
        if(sscanf(tokens[p].str, "%lu", &ret) == -1)
          Assert(0, "sscanf fault");
        return ret;

      case TK_HEX:
        if(sscanf(tokens[p].str, "%lu", &ret) == -1)
          Assert(0, "sscanf fault");
        return ret;

      case TK_REG:;
        bool flag=true;
        ret = isa_reg_str2val(tokens[p].str, &flag);
        if(flag)
          return ret;
        Assert(0, "read register failed");

      default:
        Assert(0, "a single token is not a number");
        break;
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1);
  }
  else {
    int op_pos = find_main_op(p, q);
    if(op_pos == -1)
    {

      Assert(0, "no op has been found");
    }
    word_t val1 = eval(p, op_pos - 1);
    word_t val2 = eval(op_pos + 1, q);

    switch (tokens[op_pos].type) {
      case TK_PLUS: return val1 + val2;
      case TK_MINUS: return val1 - val2;
      case TK_MUL: return val1 * val2;
      case TK_EQ: return val1 == val2;
      case TK_NEQ: return val1 != val2;
      case TK_AND: return val1 && val2;
      case TK_L: return val1 < val2;
      case TK_LE: return val1 <= val2;
      case TK_G: return val1 > val2;
      case TK_GE: return val1 >= val2;
      case TK_DIV:
      if(val2 == 0)
        Assert(0, "divide 0 error");
      return val1 / val2;
      default: Assert(0, "impossible main op type %d", tokens[op_pos].type);
    }
  }
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  return eval(0,nr_token-1);
}
