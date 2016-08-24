calc.out: calc.c
	clang calc.c -lm -o calc.out
clean: rm -f calc
