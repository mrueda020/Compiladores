lex.yy.c: Pascal.l
	flex Pascal.l

a.out: lex.yy.c
	gcc lex.yy.c -lm -ll

run: a.out
	./a.out		