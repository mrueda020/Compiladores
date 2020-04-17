analizador.tab.c: analizador.y
	bison -d analizador.y
lex.yy.c: analizador.l analizador.tab.h
	flex analizador.l
analizador.tab.o: analizador.tab.c
	gcc analizador.tab.c -c
lex.yy.o: lex.yy.c
	gcc lex.yy.c -c
a.out: analizador.tab.o lex.yy.o
	gcc  lex.yy.o analizador.tab.o  -ll -lm -ly 
run: a.out
	./a.out "test1.c"

clean:
	rm *.o
	rm analizador.tab.c
	rm lex.yy.c
	rm analizador.tab.h
	rm a.out
	clear
