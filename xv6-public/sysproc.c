#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "const.h"
#include "VGA_reg.h"

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
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
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
    if(myproc()->killed){
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


int
sys_VGA_mode_switch(void)
{
  int mode;
  if(argint(0, &mode) < 0)
    return -1;
  VGA_mode_switch((VGA_mode)mode);
  return 0;
}

int
sys_VGA_plot_pixel(void)
{
  int x;
  int y;
  uchar color;
  if(argint(0, &x) < 0)
    return -1;
  if(argint(1, &y) < 0)
    return -1;
  int temp_color;
  if(argint(2, &temp_color) < 0)
    return -1;
  color = (char)temp_color;
  VGA_plot_pixel(x, y, color);
  return 0;
}

int
sys_VGA_plot_screen(void)
{
  char* buffer;
  if(argptr(0, &buffer, mode_width(actual_mode)*mode_height(actual_mode)) < 0)
    return -1;
  VGA_plot_screen((uchar*)buffer);
  return 0;
}

int
sys_stdin_ready(void)
{
  char* c;
  if(argptr(0, &c, 1))
    return -1;
  return(stdin_ready(c));
}
