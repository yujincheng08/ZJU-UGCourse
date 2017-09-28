power: main.o input.o compute.o
	gcc main.o input.o compute.o -o power -lm

main.o: main.c main.h input.h compute.h
	gcc -c main.c

input.o: input.c input.h
	gcc -c input.c

compute.o: compute.c compute.h
	gcc -c compute.c

