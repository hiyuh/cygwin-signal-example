using `sigaction()` w/ `SA_SIGINFO` on cygwin is unusable in some cases.  
b/c the 3rd argument of signal handler which is expected as non-`NULL`  
and valid to casting to `ucontex_t *` (was casted to `void *`).

gdb can detect where signal is raised.  
but it looks that unable to show function callings to where signal is raised.  
maybe, b/c signals are managed deeply into cygwin internals.  
FYI, it looks there is no serious problem even if clang is used.

```
$ make CC=gcc info clean check
uname -a
CYGWIN_NT-6.1-WOW64 lynx 1.7.27(0.271/5/3) 2013-12-09 11:57 i686 Cygwin
gcc -v
Using built-in specs.
COLLECT_GCC=gcc
COLLECT_LTO_WRAPPER=/usr/lib/gcc/i686-pc-cygwin/4.8.2/lto-wrapper.exe
Target: i686-pc-cygwin
Configured with: /cygdrive/i/szsz/tmpp/gcc4/gcc-4.8.2-2/src/gcc-4.8.2/configure --srcdir=/cygdrive/i/szsz/tmpp/gcc4/gcc-4.8.2-2/src/gcc-4.8.2 --prefix=/usr --exec-prefix=/usr --bindir=/usr/bin --sbindir=/usr/sbin --libexecdir=/usr/libexec --datadir=/usr/share --localstatedir=/var --sysconfdir=/etc --libdir=/usr/lib --datarootdir=/usr/share --docdir=/usr/share/doc/gcc --htmldir=/usr/share/doc/gcc/html -C --build=i686-pc-cygwin --host=i686-pc-cygwin --target=i686-pc-cygwin --without-libiconv-prefix --without-libintl-prefix --enable-shared --enable-shared-libgcc --enable-static --enable-version-specific-runtime-libs --enable-bootstrap --disable-__cxa_atexit --with-dwarf2 --with-arch=i686 --with-tune=generic --disable-sjlj-exceptions --enable-languages=ada,c,c++,fortran,java,lto,objc,obj-c++ --enable-graphite --enable-threads=posix --enable-libatomic --enable-libgomp --disable-libitm --enable-libquadmath --enable-libquadmath-support --enable-libssp --enable-libada --enable-libjava --enable-libgcj-sublibs --disable-java-awt --disable-symvers --with-ecj-jar=/usr/share/java/ecj.jar --with-gnu-ld --with-gnu-as --with-cloog-include=/usr/include/cloog-isl --without-libiconv-prefix --without-libintl-prefix --with-system-zlib --libexecdir=/usr/lib
Thread model: posix
gcc version 4.8.2 (GCC)
gdb -v
GNU gdb (GDB) 7.6.50.20130728-cvs (cygwin-special)
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-pc-cygwin".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".

rm -f test.exe
rm -f test.exe.stackdump
gcc -g -O0 -std=gnu99 -Wall -o test.exe test.c
./test.exe
assertion "secret != NULL" failed: file "test.c", line 30, function: signal_handler
Makefile:2: recipe for target 'check' failed
make: *** [check] Aborted (core dumped)

$ gdb ./test.exe
GNU gdb (GDB) 7.6.50.20130728-cvs (cygwin-special)
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-pc-cygwin".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
..
Reading symbols from /home/hiyuh/git-repos/cygwin-signal-example/test.exe...done.
(gdb) r
Starting program: /home/hiyuh/git-repos/cygwin-signal-example/test.exe
[New Thread 5944.0x5d8]
[New Thread 5944.0x1464]

Program received signal SIGSEGV, Segmentation fault.
0x004011a2 in f10 () at test.c:7
7               if (raise(SIGSEGV) != 0) {
(gdb) bt
#0  0x004011a2 in f10 () at test.c:7
#1  0x758a1194 in WaitForSingleObjectEx () from /cygdrive/c/Windows/syswow64/kernel32.dll
#2  0x758a1148 in WaitForSingleObject () from /cygdrive/c/Windows/syswow64/kernel32.dll
#3  0x610dc1a0 in sig_send(_pinfo*, siginfo_t&, _cygtls*) () from /usr/bin/cygwin1.dll
#4  0x610d960d in _pinfo::kill(siginfo_t&) () from /usr/bin/cygwin1.dll
#5  0x0028a948 in ?? ()
Backtrace stopped: previous frame inner to this frame (corrupt stack?)
(gdb) q
A debugging session is active.

        Inferior 1 [process 5944] will be killed.

Quit anyway? (y or n) y
error return /netrel/src/gdb-7.6.50-4/gdb/windows-nat.c:1289 was 5

$ make CC=clang info clean check
uname -a
CYGWIN_NT-6.1-WOW64 lynx 1.7.27(0.271/5/3) 2013-12-09 11:57 i686 Cygwin
clang -v
clang version 3.1 (branches/release_31)
Target: i386-pc-cygwin
Thread model: posix
gdb -v
GNU gdb (GDB) 7.6.50.20130728-cvs (cygwin-special)
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-pc-cygwin".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".

rm -f test.exe
rm -f test.exe.stackdump
clang -g -O0 -std=gnu99 -Wall -o test.exe test.c
./test.exe
assertion "secret != NULL" failed: file "test.c", line 30, function: signal_handler
Makefile:2: recipe for target 'check' failed
make: *** [check] Aborted (core dumped)

$ gdb ./test.exe
GNU gdb (GDB) 7.6.50.20130728-cvs (cygwin-special)
Copyright (C) 2013 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "i686-pc-cygwin".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word".
..
Reading symbols from /home/hiyuh/git-repos/cygwin-signal-example/test.exe...DW_FORM_strp pointing outside of .debug_str section [in module /home/hiyuh/git-repos/cygwin-signal-example/test.exe]
(no debugging symbols found)...done.
(gdb) r
Starting program: /home/hiyuh/git-repos/cygwin-signal-example/test.exe
[New Thread 5700.0x138c]
[New Thread 5700.0x19a0]

Program received signal SIGSEGV, Segmentation fault.
0x004012fa in f10 ()
(gdb) bt
#0  0x004012fa in f10 ()
#1  0x758a1194 in WaitForSingleObjectEx () from /cygdrive/c/Windows/syswow64/kernel32.dll
#2  0x758a1148 in WaitForSingleObject () from /cygdrive/c/Windows/syswow64/kernel32.dll
#3  0x610dc1a0 in sig_send(_pinfo*, siginfo_t&, _cygtls*) () from /usr/bin/cygwin1.dll
#4  0x610d960d in _pinfo::kill(siginfo_t&) () from /usr/bin/cygwin1.dll
#5  0x00820138 in ?? ()
Backtrace stopped: previous frame inner to this frame (corrupt stack?)
(gdb) q
A debugging session is active.

        Inferior 1 [process 5700] will be killed.

Quit anyway? (y or n) y
error return /netrel/src/gdb-7.6.50-4/gdb/windows-nat.c:1289 was 5
```
