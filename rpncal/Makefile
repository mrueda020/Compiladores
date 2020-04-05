rpncal.tab.c: rpncal.y
	bison rpncal.y
a.out: rpncal.tab.c
	gcc rpncal.tab.c -lm
run: a.out
	./a.out    
make clean:
	rm a.out
	rm rpncal.tab.c

