CC:=gcc
CFLAGS:=-std=c99 -Wall -Wextra -O2 -c
LD=$(CC)
LDFLAGS:=-lm

all: lcg.o test.exe

lcg.o: lcg.c lcg.h
	$(CC) $(CFLAGS) $< -o $@
      
test.exe: lcg_test.o lcg.o
	$(LD) -o $@ $^ $(LDFLAGS) 
	
lcg_test.o: lcg_test.c
	$(CC) $(CFLAGS) $< -o $@
    
clean:
	del *.o
	del *.exe    
