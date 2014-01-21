using `sigaction()` w/ `SA_SIGINFO` on cygwin is unusable in some cases.  
b/c the 3rd argument of signal handler which is expected as non-`NULL`  
and valid to casting to `ucontex_t *` (was casted to `void *`).

```
$ uname -a
CYGWIN_NT-6.1-WOW64 lynx 1.7.27(0.271/5/3) 2013-12-09 11:57 i686 Cygwin

$ make
gcc -g -O0 -std=gnu99 -Wall -o test.exe test.c
./test.exe
assertion "secret != NULL" failed: file "test.c", line 30, function: signal_handler
Makefile:2: recipe for target 'check' failed
make: *** [check] Aborted (core dumped)

$ cat test.exe.stackdump
Stack trace:
Frame     Function  Args

```
