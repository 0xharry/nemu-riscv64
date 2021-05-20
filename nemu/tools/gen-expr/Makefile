APP=ge

$(APP): gen-expr.c
	gcc -g -Wall -Werror -o $@ $<

tmp : /home/harry/ics2020/nemu/temp/.code.c
	gcc -g /home/harry/ics2020/nemu/temp/.code.c -o /home/harry/ics2020/nemu/temp/.expr

.PHONY: clean
clean:
	-rm $(APP) 2> /dev/null
