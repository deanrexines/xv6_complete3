#include "types.h"
#include "stat.h"
#include "user.h"
#include "signal.h"

#define time_ 10000

static int count = 0;
 
void 
handle_signal(siginfo_t signum)
{
    if (count < time_) {
        count++;
    } else {
        // skip over the next instruction
        int *sig = (int *)&signum;
        *(sig + 4*sizeof(int*)) += 4;
    }
 }
 
 int main(int argc, char *argv[])
 {

	signal(SIGFPE, &handle_signal);

    int x = 0;
    x /= x; 
	printf(1, "Traps Performed: %d\n", count);
	printf(1, "Total Elapsed Time: %d\n", uptime());
	printf(1, "Average Time Per Trap: %d\n", count/uptime());
 
 	exit();
}