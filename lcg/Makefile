CC:=gcc
CFLAGS:=-std=c99 -Wall -Wextra -O2 -c
LD=$(CC)
LDFLAGS:=-lm

all: lcg.o test

lcg.o: lcg.c lcg.h
	$(CC) $(CFLAGS) $< -o $@
      
test: lcg_test.o lcg.o
	$(LD) -o $@ $^ $(LDFLAGS) 
	
lcg_test.o: lcg_test.c
	$(CC) $(CFLAGS) $< -o $@
    
clean:
	rm -f *.o
	rm -f *.exe
	rm -f test
