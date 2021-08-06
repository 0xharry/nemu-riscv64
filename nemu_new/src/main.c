void init_monitor(int, char *[]);
void engine_start();
int is_exit_status_bad();

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "monitor/debug/expr.h"
// extern uint64_t expr(char *e, bool *success);
// int test_expr(char*);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  init_monitor(argc, argv);
  // printf("Test start\n");
  // test_expr("/home/dzw/ics-pa/nemu/tools/gen-expr/input");
  // printf("Test end\n");
  /* Start engine. */
  engine_start();

  return is_exit_status_bad();
}


// void test_expr(){
//   //读入input文件
//   FILE *fp;
//   char * buf = malloc(512*sizeof(unsigned));
//   unsigned result=0;
//   bool success = 0;
//   if((fp=fopen("/home/dzw/ics-pa/nemu/tools/gen-expr/input","r"))==NULL)
//     printf("fopen failed\n");
//   if(fscanf(fp, "%d %s", &result,buf));
//   fclose(fp);
//   printf("res:%d  buf:%s\n",result,buf);
//   int ret = expr(buf,&success);
//   if(ret==0) printf("test failed\n");
//   else printf("test successfully\n");
// }



int test_expr(char * filename){
  FILE * fp;
  char * p,buf[1024];
  int lineNum = 0;

  if((fp = fopen(filename,"r"))==NULL){
      printf("open failed\n");
      return -1;
  }


  //fgets从指定的流读取一行
  for(lineNum=0;fgets(buf,sizeof(buf),fp)!=NULL;lineNum++){
      if ((p = strchr(buf, '\n')) == NULL) {
            p = buf + strlen(buf);
        }
        if (p > buf && p[-1] == '\r') {
            --p;
        }
        *p = '\0';
        for (p = buf; *p != '\0'&& isspace((int)*p); ++p) {
            ;
        }
        if (*p == '\0') {
            continue;
        }
    
    printf("%s\n",p);


    char *arg1 = strtok(p, " ");
    char *arg2 = arg1+strlen(arg1)+1;

    //printf("LineNum : %d , arg1 : %s  ,arg2 : %s\n",lineNum,arg1,arg2);
    
    int num = strtol(arg1,NULL,10);
    
    bool succ;

    int predict = expr(arg2,&succ);

    //printf("result = %d , predict = %d\n",num,predict);
    if(predict == num)
      printf("PASS\n");
    else
    {
      printf("FAIL\n");
    }
    
  }
  fclose(fp);
  return 0;
}