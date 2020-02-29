all: file1.c file2.c file3.c
	gcc -o file1 file1.c
	gcc -o file2 file2.c
	gcc -o file3 file3.c
prob1: file1.c
	gcc -o file1 file1.c 

prob2:	file2.c
	gcc -o file2 file2.c

prob3: file3.c
	gcc -o file3 file3.c
clean: 
	rm file1; rm file2; rm file3
