#include <isa.h>
#include "expr.h"
/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <memory/paddr.h>
enum TK {
  TK_NOTYPE = 256,

  // 关系运算：
  // 优先级最低
  TK_OR,    // ||
  // 优先级++
  TK_AND,   // &&
  // 优先级++
  TK_EQ,    // ==
  TK_NEQ,   // !=
  TK_L,     // <
  TK_LE,    // <=
  TK_G,     // >
  TK_GE,    // >=

  // 加减乘除：
  // 优先级++
  TK_MINUS,   // -
  TK_PLUS,    // +
  // 优先级++
  TK_MUL,     // *
  TK_DIV,     // /

  // 括号：
  // 优先级++
  TK_LP,      // (
  TK_RP,      // )

  // 操作数：
  // 优先级++
  TK_DEC,     // Decimal integer
  TK_HEX,     // 0x Hexadecimal integer
  TK_REG,     // $  Register

  // 特殊：
  // 优先级最高
  TK_NEG,   // - negative
  TK_DEREF, // * dereference
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {
    {" +",                          TK_NOTYPE},  // spaces
    {"\\+",                         TK_PLUS},    // plus
    {"\\(",                         TK_LP},      // left parenthesis
    {"\\)",                         TK_RP},      // right parenthesis
    {"\\*",                         TK_MUL},     // multiply
    {"-",                           TK_MINUS},   // minus
    {"\\/",                         TK_DIV},     // divide
    {"==",                          TK_EQ},      // equal
    {"0x[0-9]+",                    TK_HEX},     // Hexadecimal integer
    {"[0-9]+",                      TK_DEC},     // Decimal integer
    {"\\$[\\$rstagp][apc0-9][01]?", TK_REG},     // Register
    {"!=",                          TK_NEQ},     // not equal
    {"&{2}",                        TK_AND},     // and
    {"\\|{2}",                      TK_OR},      // or
    {"<=",                          TK_LE},      // less or equal
    {">=",                          TK_GE},      // great or equal
    {"<",                           TK_L},       // less
    {">",                           TK_G},       // great
};

// 规则数量
#define NR_REGEX (sizeof(rules) / sizeof(rules[0]))

// 编译后的特定re格式
static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

// TK_NUM 个token是否不够用？
#define TK_NUM 64
static Token tokens[TK_NUM] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  // 按顺序挨个字符（串）匹配re规则
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++) {
      // 一条规则匹配成功, 并且匹配出的子串正好是position所在位置
      bool ret = regexec(&re[i], e + position, 1, &pmatch, 0);
      if (ret == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;
        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
        position += substr_len;

        switch (rules[i].token_type) {
        // 十进制、十六进制、寄存器需要保存，单独处理
        case TK_DEC:
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          tokens[nr_token].str[substr_len] = '\0';
          tokens[nr_token++].type = TK_DEC;
          break;
        case TK_HEX:
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          tokens[nr_token].str[substr_len] = '\0';
          tokens[nr_token++].type = TK_HEX;
          break;
        case TK_REG:
          strncpy(tokens[nr_token].str, substr_start + 1, substr_len - 1);
          tokens[nr_token].str[substr_len-1] = '\0';
          tokens[nr_token++].type = TK_REG;
          break;

        case TK_MINUS: case TK_PLUS:
        case TK_MUL:   case TK_DIV:
        case TK_NEQ:   case TK_EQ:
        case TK_LP:    case TK_RP:
        case TK_AND:   case TK_OR:
        case TK_LE:    case TK_GE:
        case TK_L:     case TK_G:
          tokens[nr_token++].type = rules[i].token_type;

        case TK_NOTYPE: break;
        default:
          Log("no match rules like \"%d\" type\nat position %d with len %d: %.*s",
              rules[i].token_type, position, substr_len, substr_len, substr_start);
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

bool check_parentheses(int p, int q) {
  if (tokens[p].type != TK_LP || tokens[q].type != TK_RP)
    return false;
  // 双指针，相遇时退出，判断左右括号是否>0且相等
  p++; q--;
  int left = 1, right = 1;
  while (p <= q) {
    // 左指针
    switch (tokens[p++].type) {
      case TK_LP: left++; break;
      case TK_RP: left--; break;
      default:            break;
    }
    // 相遇时退出
    if (p > q) break;
    // 右指针
    switch (tokens[q--].type) {
      case TK_LP: left++; break;
      case TK_RP: left--; break;
      default:            break;
    }
  }

  if (left > 0 && left == right)
    return true;
  else
    return false;
}

#include <limits.h>
#define BAD_RET(msg,st,ret) do{                                     \
          printf(msg);                                              \
          *state=st;                                                \
          return ret;                                               \
        }while(0)

// 如果当前op优先级更低，登记；如果优先级相同，取最后出现的同级op
#define OP_STATE_UPDATE(op) do{                                     \
          if(ret_op_type >= op) {                                   \
            ret_op_type = tokens[p-1].type;                         \
            ret_op_pos = p-1;                                       \
          }                                                         \
        }while(0)

#define BAD_OP INT_MAX

int find_main_op(int p, int q, int* state) {
  int ret_op_type = BAD_OP;  // 当前op类型
  int num_parentheses = 0;    // 当前嵌套在几层括号中
  int ret_op_pos = BAD_OP;    // 最低op优先级下标

  while (p <= q) {
    // 跳过括号中的所有op
    if(tokens[p].type == TK_LP) {
      do {
        switch (tokens[p++].type) {
        case TK_LP: num_parentheses++; break;
        case TK_RP: num_parentheses--; break;
        default:                       break;
        }
      } while ((p <= q) && (num_parentheses > 0));
      continue;
    }

    switch (tokens[p++].type) {
    // 此处不应该出现任何括号
    case TK_LP:
    case TK_RP:
      BAD_RET("find_main_op(): Invalid parenthesis\n",FIND_OP_FAIL,-1); break;

    // 优先级依次提升
    case TK_OR:
      OP_STATE_UPDATE(TK_OR);    break;

    case TK_AND:
      OP_STATE_UPDATE(TK_AND);   break;

    case TK_EQ: case TK_NEQ:
    case TK_L:  case TK_LE:
    case TK_G:  case TK_GE:
      OP_STATE_UPDATE(TK_EQ);    break;

    case TK_MINUS:
    case TK_PLUS:
      OP_STATE_UPDATE(TK_MINUS); break;

    case TK_MUL:
    case TK_DIV:
      OP_STATE_UPDATE(TK_MUL);   break;

    case TK_NEG:
    case TK_DEREF:
      OP_STATE_UPDATE(TK_NEG); break;

    // 非op tokens，跳过
    default:                     break;
    }
  }

  if (ret_op_pos == BAD_OP)
    BAD_RET("find_main_op(): find no valid op\n", FIND_OP_FAIL,-1);
  else
    return ret_op_pos;
}

word_t eval(int p, int q, int *state)
{
  // 错误返回main_loop
  if (*state != VALID_RET)  return -1;

  if (p > q) {
    BAD_RET("eval(): BAD_EXPR(p > q)\n", BAD_EXPR, 1);
  }
  else if (p == q) {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    word_t ret;
    switch (tokens[p].type) {
    case TK_DEC:
      if (sscanf(tokens[p].str, "%lu", &ret) == -1)
        BAD_RET("eval(): TK_DEC, SSCANF_FAIL\n",SSCANF_FAIL,1);
      return ret;

    case TK_HEX:
      if (sscanf(tokens[p].str, "%lx", &ret) == -1)
        BAD_RET("eval(): TK_HEX, SSCANF_FAIL\n",SSCANF_FAIL,1);
      return ret;

    case TK_REG:;
      bool flag = true;
      ret = isa_reg_str2val(tokens[p].str, &flag);
      if (!flag)
        BAD_RET("eval(): REG_FAIL, SSCANF_FAIL\n",REG_FAIL,1);
      return ret;

    default:
      BAD_RET("eval(): invalid (single)token type\n",BAD_EXPR,1);
      break;
    }
  }
  else if (check_parentheses(p, q) == true) {
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(p + 1, q - 1, state);
  }
  else {
    int op_pos = find_main_op(p, q, state);
    if (*state != VALID_RET)  return 1;

    // 如果op是取地址*或负-（最高优先级的op）
    // 此时应该只剩右值，单独处理
    switch (tokens[op_pos].type) {
    case TK_DEREF:
      if(p != op_pos)
        BAD_RET("eval(): REG_FAIL, SSCANF_FAIL\n",REG_FAIL,1);
      return paddr_read(eval(op_pos + 1, q, state), 4);

    case TK_NEG:
      if(p != op_pos)
        BAD_RET("eval(): dereference* and negative- should be on the left\n",BAD_EXPR,-1);
      return -eval(op_pos + 1, q, state);

    default: break;
    }

    word_t val1 = eval(p, op_pos - 1, state);
    word_t val2 = eval(op_pos + 1, q, state);
    if (*state != VALID_RET)  return 1;

    switch (tokens[op_pos].type)
    {
    case TK_MINUS: return val1 -  val2;
    case TK_PLUS:  return val1 +  val2;
    case TK_MUL:   return val1 *  val2;
    case TK_NEQ:   return val1 != val2;
    case TK_AND:   return val1 && val2;
    case TK_OR:    return val1 || val2;
    case TK_EQ:    return val1 == val2;
    case TK_LE:    return val1 <= val2;
    case TK_GE:    return val1 >= val2;
    case TK_L:     return val1 <  val2;
    case TK_G:     return val1 >  val2;
    case TK_DIV:
      if (*state != VALID_RET) return -1;
      if (val2 == 0)
        BAD_RET("eval(): divide by 0\n",DIV_BY_0,-1);
      return val1 / val2;

    default:
      BAD_RET("eval(): impossible main op type\n",BAD_EXPR,-1);
    }
  }
}

word_t expr(char *e, int *state) {
  *state = VALID_RET;
  if (!make_token(e))
    BAD_RET("expr(): make_token fail\n",MAKE_FAIL,0);

  // 区分 *乘/取地址，和 -减/负号
  for (int i = 0; i < nr_token; i++) {
    if ((tokens[i].type == TK_MUL) &&
        (i == 0 || tokens[i - 1].type <= TK_DIV))
      tokens[i].type = TK_DEREF;
    if ((tokens[i].type == TK_MINUS) &&
        (i == 0 || tokens[i - 1].type <= TK_DIV))
      tokens[i].type = TK_NEG;
  }

  return eval(0, nr_token - 1, state);
}
