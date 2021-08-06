#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <stdlib.h>
#include <memory/vaddr.h>



enum {
  TK_NOTYPE = 256, TK_EQ,

  //pa1.2
  TK_DEC,
  //pa1.3
  TK_AND,TK_ORR,TK_NEQ,TK_NOT,TK_HEX,TK_REG,TK_LE,TK_GE,TK_DEREF,TK_MINUS
};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces  识别即丢弃，例外，不需记录
  {"\\+", '+'},         // plus
  {"==", TK_EQ},        // equal

  //pa1.2
  {"\\*",'*'},
  {"-",'-'},
  {"/",'/'},
  {"\\(",'('},
  {"\\)",')'},
  {"0|[1-9][0-9]*",TK_DEC},

  //pa1.3
  {"&&",TK_AND},
  {"\\|\\|",TK_ORR},
  {"!=",TK_NEQ},
  {"!",TK_NOT},
  {"0[Xx][0-9A-Fa-f]+",TK_HEX},
  {"(0|s[0-11]|t[0-6]|a[0-7]|sp|gp|ra)",TK_REG},
  {"<=",TK_LE},
  {">=",TK_GE},
  {"<",'<'},
  {">",'>'},
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    //编译 re存放编译好的，rules.regex指向写好的正则表达式的指针
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[512];
} Token;

static Token tokens[32] __attribute__((used)) = {};//按顺序存放已被识别出的token信息
static int nr_token __attribute__((used))  = 0;  //已被识别出的token数目

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;
  //position当前处理到的位置
  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {

      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
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
          case TK_NOTYPE:
            break;
          
          //单目运算符
          case '+':
          case '-':
          case '*':
          case '/':
          case '(':
          case ')':
          case TK_NOT:
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
          
          //其余的多个字符
          default:
            tokens[nr_token].type = rules[i].token_type;
            memset(tokens[nr_token].str,'\0',512);
            Assert(substr_len<512,"len of token is %d\n",substr_len);
            strncpy(tokens[nr_token].str,substr_start,substr_len);//fix error
            tokens[nr_token].str[substr_len]='\0';
            nr_token++;
            break;
        }
        break;
      }
    }
    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }
  printf("NR_TOKEN : %d\n",nr_token);
  return true;
}


//pa1.2
static u_int64_t eval(int p,int q);
static bool check_parentheses(int p,int q);
static u_int64_t getMainOpt(int p,int q);
static bool is_opt(u_int64_t x);
static u_int64_t opt_pri(int x);
static bool is_deref(int x);

static u_int64_t getMainOpt(int p,int q){  //bug
  int result=p;
  int pos = p;
  int sum =0;
  int curPriority = 0;
  for(;pos<=q;pos++){
    int tp = tokens[pos].type;
    //注意if的顺序，如果先判断sum，那么当前位为括号则会跳过
    if(tp=='('){
      sum++;
      continue;
    }
    if(tp==')'){
      sum--;
      continue;
    }

    if(!is_opt(tp))
      continue;
    
    if(sum!=0)
      continue;
    else{
      if(opt_pri(tp)>=curPriority){
      curPriority = opt_pri(tp);
      result = pos;
      }
      else 
        continue;
    }

  }
  //printf("main opt @ position %d\n",result);
  return result;
}

//判断是否是运算符
static bool is_opt(u_int64_t x){
  switch (x){
    case TK_HEX:
    case TK_REG:
    case TK_DEC:
    case TK_NOTYPE:
      return false;
    default:
      return true;
  }
}

//数字越高优先级越高
static u_int64_t opt_pri(int x){
  switch (x){
    
    case '(':
    case ')':
      return 2;

    case TK_NOT:
    case TK_DEREF:
    case TK_MINUS:
      return 3;

    case '*':
    case '/':
      return 4;

    case '+':
    case '-':
      return 5;

    case '>':
    case '<':
    case TK_LE:
    case TK_GE:
    case TK_NEQ:
    case TK_EQ:
      return 6;

    case TK_AND:
      return 7;

    case TK_ORR:
      return 8;
    default:
      return -1;
  }
}

//判断是否是解引用之前可能出现的符号
static bool is_deref(int x){
  switch (x){
      case TK_DEC:
      case TK_REG:
      case TK_HEX:
      case ')':
      return false;
    default:
      return false;
  }
}

static bool is_MINUS(int x){
  switch (x)
  {
    case TK_DEC:
    case TK_REG:
    case TK_HEX:
    case ')':
      return false;
    break;
  
    default:
      return true;
  }
}

//检查表达式是否被一对括号包围
static bool check_parentheses(int p,int q){
  if(tokens[p].type!='(' || tokens[q].type!=')')
    return false;

  int i,sum=0;
  for(i=p+1;i<q;i++){
    int tp = tokens[i].type;
    if(tp=='(')
      sum++;
    else 
    if(tp==')') 
      sum--;
    if(sum<0)
      return false;
  }


  return sum==0;
}


static u_int64_t eval(int p,int q){
  //static int nums=0;
  if(p>q){
    printf("Bad expression\n");
    return 0;
  }
  else if(p==q){
    if(tokens[p].type == TK_REG){
      bool flag = true;
      u_int64_t answer = isa_reg_str2val(tokens[p].str,&flag);
      if(!flag){
        printf("invalid reg : input like $eax\n");
        return 0;
      }else{
        return answer;
      }
    }
    else if(tokens[p].type == TK_HEX){
      int hexval = 0;
      sscanf(tokens[p].str,"0x%x",&hexval);
      return hexval;
    }
    else{
          u_int64_t number = strtol(tokens[p].str,NULL,10);
          return number;
    }

  }

  else if(check_parentheses(p,q)==true){
    
    // printf("p : %d , q : %d\n",p,q);
    // nums++;
    return eval(p+1,q-1);
  }

  else{
    u_int64_t op = getMainOpt(p,q);  

    //将解引用和负号独立出来考虑，且这只对一个数进行操作
    if(p == op || tokens[op].type==TK_MINUS || tokens[op].type==TK_DEREF){
      u_int64_t val = eval(p+1,q);
      switch (tokens[op].type)
      {
        case TK_MINUS:
          return -val;
        case TK_DEREF:
          return vaddr_read(val,4);
          
        default:
          return -1;
          break;
      }
    }


    u_int64_t val1 = eval(p,op-1);
    u_int64_t val2 = eval(op+1,q);

    switch(tokens[op].type){
      case '+':
        return val1+val2;
      case '-':
        return val1-val2;
      case '*':
        return val1*val2;
      case '/':
        if(val2==0){
          Assert(0,"Zero div is wrong!\n");
        }
        return val1/val2;
      case TK_EQ:
        return val1==val2;
      case TK_GE:
        return val1>=val2;
      case TK_LE:
        return val1<=val2;
      case TK_AND:
        return val1&&val2;
      case TK_ORR:
        return val1||val2;
      case '<':
        return val1<val2;
      case '>':
        return val1>val2;
      case TK_NEQ:
        return val1!=val2;
      default:
        Assert(0,"invalid operation\n");
    }
  }
}

uint64_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    printf("make token error!\n");
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  *success = true;
  //printf("make token successfully!\n");


  //printf("Enter check parentheses \n");
  //bool res = check_parentheses(0,nr_token-1);
  // if(res)
  //   printf("check_parenthese successfully\n");
  // else
  // {
  //   printf("check_parenthese fail\n");
  // }

  // printf("Enter get main opt\n");
  //int index = getMainOpt(0,nr_token-1);
  //printf("main opt @ position %d\n",index);

  int i;
  for(i = 0;i<nr_token;i++){
    //这二者都在原有基础上修改语义
    if(tokens[i].type=='*' && (i==0 || is_deref(tokens[i-1].type))){
      tokens[i].type = TK_DEREF;
    }
    if(tokens[i].type=='-' && (i==0 || is_MINUS(tokens[i-1].type))){
      tokens[i].type = TK_MINUS;
    }

  }

  // printf("Enter eval final value\n");
  uint64_t val = eval(0,nr_token-1);
  //printf("result is : %d\n",val);
  

  return val;
}