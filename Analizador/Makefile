analizador.tab.c: analizador.y
	bison -d analizador.y
	clear
lex.yy.c: analizador.l analizador.tab.h
	flex analizador.l
	clear
analizador.tab.o: analizador.tab.c
	gcc analizador.tab.c -c
	clear
lex.yy.o: lex.yy.c
	gcc lex.yy.c -c
	clear
a.out: analizador.tab.o lex.yy.o
	gcc  lex.yy.o analizador.tab.o  -ll -lm -ly 
	clear
run: a.out
	./a.out "test1.c"

clean:
	rm *.o
	rm analizador.tab.c
	rm lex.yy.c
	rm analizador.tab.h
	rm a.out
	clear
