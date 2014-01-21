#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

static void f10() {
	if (raise(SIGSEGV) != 0) {
		perror("raise");
		exit(EXIT_FAILURE);
	}
	return;
}

#define fdecl(a, b)      \
	static void f##a() { \
		return f##b();   \
	}
fdecl( 9, 10)
fdecl( 8,  9)
fdecl( 7,  8)
fdecl( 6,  7)
fdecl( 5,  6)
fdecl( 4,  5)
fdecl( 3,  4)
fdecl( 2,  3)
fdecl( 1,  2)
fdecl( 0,  1)

static void signal_handler(int sig, siginfo_t *info, void *secret) {
	assert(secret != NULL); // FIXME: OMG!!1
	return;
}

static void register_signal_handler(void) {
	struct sigaction sa;
	sa.sa_sigaction = (void*)signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);
	return;
}

int main(int argc, char *argv[]) {
	register_signal_handler();
	f0();
	return 0;
}
