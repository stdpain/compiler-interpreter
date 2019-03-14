TARGET = main
CC=gcc

OBJS = \
  lex.yy.o\
  y.tab.o\
  main.o\
  create.o\
  interpreter.o\
  ../memory/mem.o\
  eval.o\
  string_pool.o

CFLAGS = -c -g -Wall -Wswitch-enum  -pedantic -DDEBUG
INCLUDES = \
	-I../include

$(TARGET):$(OBJS)
	cd ../memory; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm

eval.o: eval.c ../include/eval.h
	 $(CC) -c -g $*.c $(INCLUDES)
	
y.tab.h : ms.y
	bison --yacc -dv ms.y
y.tab.c : ms.y
	bison --yacc -dv ms.y
lex.yy.c : ms.l ms.y y.tab.h
	flex ms.l
y.tab.o: y.tab.c ../include/create.h ../include/MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
string_pool.o: y.tab.c ../include/string_pool.h ../include/MEM.h ../include/interpreter.h string_pool.c
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c ../include/create.h ../include/MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
interpreter.o:interpreter.c ../include/interpreter.h ../include/create.h
	$(CC) -c -g $*.c $(INCLUDES)
./memory/mem.o:
	cd ../memory; $(MAKE);
