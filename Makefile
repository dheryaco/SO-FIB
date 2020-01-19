all: test directorios directorios2

test: test.c
	gcc -o test test.c

directorios: directorios.c
	gcc -o directorios directorios.c

directorios2: directorios2.c
	gcc -o directorios2 directorios2.c
	
clean:
	rm test directorios clab2.tar.gz

tar:
	tar zcvf clab2.tar.gz Makefile *.c respuestas.txt 
