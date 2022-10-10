#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "sysinfo.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
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

uint64
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

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// 新增的trace 函数
uint64
sys_trace(void)
{
  int temp;
  if(argint(0, &temp) < 0){
    return -1;
  }
  // 需提前在proc.h中声明
  myproc()->mask = temp;
  return 0;
}

// 新的sysinfo函数
uint64
sys_sysinfo(void)
{
  // 存放用户态传入的参数
  uint64 temp;
  if(argaddr(0, &temp) < 0){
    return -1;
  }
  struct proc *pTemp = myproc();
  struct sysinfo infoTemp;

  // 调用自己写的计算剩余空间(kalloc.c)，计算空闲进程数(proc.c)，计算可用文件描述符(proc.c)。函数名在defs.h
  infoTemp.freemem = freemem_calculate();
  infoTemp.nproc = nproc_calculate();
  infoTemp.freefd = freefd_calculate();

  if(copyout(pTemp->pagetable, temp, (char *)&infoTemp, sizeof(infoTemp)) < 0){
    return -1;
  }
  return 0;
}
