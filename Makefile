check: test.exe
	./test.exe

test.exe: test.c
	gcc -g -O0 -std=gnu99 -Wall -o test.exe test.c

.PHONY: clean

clean:
	rm -f test.exe
	rm -f test.exe.stackdump

