#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return proc->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = proc->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;
  
  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(proc->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;
  
  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// Halt (shutdown) the system by sending a special
// signal to QEMU.
// Based on: http://pdos.csail.mit.edu/6.828/2012/homework/xv6-syscall.html
// and: https://github.com/t3rm1n4l/pintos/blob/master/devices/shutdown.c
int
sys_halt(void)
{
  char *p = "Shutdown";
  for( ; *p; p++)
    outw(0xB004, 0x2000);
  return 0;
}

// Register a signal handler
sighandler_t
sys_register_signal_handler(void)
{
    int sig;
    sighandler_t handler;
    void (*tramp)(void);
    // get the signal from arg
    /*
    cprintf("In sys_signal\n");
    int i;
    for (i = 0; i < 8; i++) {
        if (argint(i, &sig) < 0) {
            cprintf("argint failed\n");
        }
        cprintf("argint %d %d\n", i, sig);
    }
    */
    if (argint(0, &sig) < 0 || argint(1, (int *)&handler) < 0 || argint(2, (int *)&tramp)) {
        return (sighandler_t)-1;
    }
    //cprintf("Got args! %d %x\n", sig, handler);
    sighandler_t prev = proc->sighandlers[sig];
    proc->tramp = tramp;
    proc->sighandlers[sig] = handler;
    //cprintf("Signal handler set.\n");
    return prev;
}

// New alarm for a process (overwrites any running one if existing)
// Return 0 on success, -1 on error
int
sys_alarm(void)
{
    int sec;
    if (argint(0, &sec) < 0) {
        cprintf("Argument error\n");
        return -1;
    }
    if (sec > 0) {
        proc->ticks = sec*100;
    } else {
        return -1;
    }
    return 0;
}