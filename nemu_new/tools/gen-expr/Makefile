APP=gen-expr

$(APP): gen-expr.c
	gcc -O2 -g -rdynamic -Wall -Werror -o $@ $<

.PHONY: clean
clean:
	-rm $(APP) 2> /dev/null
