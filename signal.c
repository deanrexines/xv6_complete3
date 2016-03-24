#include "types.h"
#include "user.h"

void func(siginfo_t info) {
    printf(1, "Test signal handler\n");
}

int
main(int argc, char *argv[])
{
    printf(1, "Calling system call signal\n");
    //signal();
    signal(SIGFPE, &func);
    exit(); // Try return next
}