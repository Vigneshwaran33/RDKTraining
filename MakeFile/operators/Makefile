
 
cc=gcc

Target:main

main: main.o lib_calc.a
	$(cc) $< -o $@ -L. -l_calc
	
lib_calc.a: arithmetic.o relational.o
	ar rcs lib_calc.a $^
	
main.o:main.c
	$(cc) -c $<
	
arithmetic.o:arithmetic.c
	$(cc) -c $<
	
relational.o:relational.c
	$(cc) -c $<
	
clean:
	rm *.o *.a main


