check: test.exe
	./test.exe

test.exe: test.c
	$(CC) -g -O0 -std=gnu99 -Wall -o test.exe test.c

.PHONY: info clean

info:
	uname -a
	$(CC) -v
	gdb -v

clean:
	rm -f test.exe
	rm -f test.exe.stackdump

