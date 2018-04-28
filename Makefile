all: watcompiler clean run

watcompiler: watcompiler.c watcompiler.l watcompiler.y watcompiler.h
	bison -d watcompiler.y
	flex -o watcompiler.lex.c watcompiler.l
	gcc -Wall watcompiler.c watcompiler.tab.c watcompiler.lex.c -o $@ -g

clean:
	rm watcompiler.tab.c watcompiler.lex.c watcompiler.tab.h

run:
	./watcompiler
