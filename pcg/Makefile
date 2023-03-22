CC:=gcc
CFLAGS:=-std=c99 -Wall -Wextra -O2 -c
LD=$(CC)
LDFLAGS:=-lm

all: pcg.o test

pcg.o: pcg.c pcg.h
	$(CC) $(CFLAGS) $< -o $@
      
test: pcg_test.o pcg.o
	$(LD) -o $@ $^ $(LDFLAGS) 
	
pcg_test.o: pcg_test.c
	$(CC) $(CFLAGS) $< -o $@
    
clean:
	rm -f *.o
	rm -f *.exe
	rm -f test
