#C Complier
CC = wcl386

#Source files:
SOURCES = main.c app.c index.c list.c library.c queryExp.c search.c

#output files name:
DICT = dict.exe

#Options
CFLAGS = -bcl=pmodew -zq /fe=$(DICT)

all:dict.exe
    cls
    echo Complie completed!
    echo Written by YU Jincheng (3150101155)

dict.exe : $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES)
    
clean: .SYMBOLIC
	- del *.obj
    - del $(DICT)