CC:=gcc
CFLAGS:=-std=c99 -Wall -Wextra -O3 -c
LD=$(CC)
LDFLAGS:=-lm
TESTU01_INCFLAGS=-I${HOME}/include
TESTU01_LDFLAGS=-L${HOME}/lib -ltestu01 -lmylib -lprobdist -lwsock32

all: pcg.o test

pcg.o: pcg.c pcg.h
	$(CC) $(CFLAGS) $< -o $@
      
test: pcg_test.o pcg.o
	$(LD) -o $@ $^ $(LDFLAGS) 
	
pcg_test.o: pcg_test.c
	$(CC) $(CFLAGS) $< -o $@
	
pcg_testu01.o: pcg_testu01.c
	$(CC) $(CFLAGS) $(TESTU01_INCFLAGS) $< -o $@
	
testu01: pcg_testu01.o pcg.o
	$(LD) -o $@ $^ $(LDFLAGS) $(TESTU01_LDFLAGS)
    
clean:
	rm -f *.o
	rm -f *.exe
	rm -f test
	rm -f testu01
