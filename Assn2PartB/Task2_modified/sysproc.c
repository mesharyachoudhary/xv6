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

int sys_wait2(void) {
  int *retime, *rutime, *stime;
  int *ctime;
  if (argptr(0, (void*)&retime, sizeof(retime)) < 0)
    return -1;
  if (argptr(1, (void*)&rutime, sizeof(retime)) < 0)
    return -1;
  if (argptr(2, (void*)&stime, sizeof(stime)) < 0)
    return -1;
  if (argptr(3, (void*)&ctime, sizeof(ctime)) < 0)
    return -1;
  return waitstats(retime, rutime, stime, ctime);
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

int sys_draw(void){
char *a="                                                                88\n"  
"                                                                88\n"  
"                                                                88\n"  
"88,dPYba,,adPYba,  ,adPPYYba, 8b,dPPYba, 8b       d8  ,adPPYba, 88\n"  
"88P     88      8a         Y8 88P     Y8  8b     d8  a8P_____88 88\n"  
"88      88      88 ,adPPPPP88 88           8b   d8   8PP        88\n"  
"88      88      88 88,    ,88 88            8b,d8     8b,   ,aa 88\n"  
"88      88      88   8bbdPaY8 88              8         Ybbd8   88\n"; 
// pic is the size of picture buffer
  int pic = 0;
  while(a[pic]!='\0'){
      pic++;
  }
  char* buf;
  int size;

  if(argint(1,&size) < 0){
    return -1;
  }

  // buffer will not be valid  
  if(argptr(0,(char**)&buf,size) < 0){
    return -1;
  }
  if(pic>size)return -1;
  int index=0;
  // storing picture characters in buffer
  while(index<pic){
    buf[index]=a[index];
    index++;
  }
  // returning size of picture
  return pic;
}

int sys_set_prio(void) {
  int priority;
  argint(0, &priority);
  return set_prio(priority);
}

int sys_yield(void) {
  yield();
  return 0;
}