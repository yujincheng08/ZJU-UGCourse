CC = gcc
OBJECTS = myshell.o innercmd.o cmdparser.o prcsctrl.o
CFLAGS = -O2
myshell: $(OBJECTS)
clean:
	rm -f *.o
	rm -f myshell

myshell.o: myshell.c myshell.h innercmd.h cmdparser.h prcsctrl.h

innercmd.o: innercmd.c innercmd.h prcsctrl.h

cmdparser.o: cmdparser.c cmdparser.h 

prcsctrl.o: prcsctrl.c prcsctrl.h innercmd.h
