#include "types.h"
#include "user.h"

void
func(siginfo_t info) {
    printf(1, "Test signal handler\n");
}

int
main(int argc, char *argv[])
{
    printf(1, "System call signal...\n");
    signal(SIGFPE, &func);
    printf(1, "Done.\n");
    exit();
}