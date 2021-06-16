APP=ge
GE_PATH = /home/harry/ics2020/nemu/tools/gen-expr/

$(APP): gen-expr.c
	gcc -g -Wall -Werror -o $@ $<

tmp : $(GE_PATH).code.c
	gcc -g $(GE_PATH).code.c -o $(GE_PATH).expr

.PHONY: clean
clean:
	-rm $(APP) 2> /dev/null
