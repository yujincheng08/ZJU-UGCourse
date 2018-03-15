CC=gcc
LDLIBS=-lm
LEX=lex
YACC=yacc
YFLAGS=-d
TARGET=expr
OBJS=scanner.o parser.o expr.o
TABH=y.tab.h
default: $(TARGET)
$(TARGET): $(OBJS)
expr.o: parser.o
scanner.o: scanner.l parser.o $(TABH)
parse.o: parse.y
clean:
	$(RM) $(OBJS) $(TARGET) $(TABH)
