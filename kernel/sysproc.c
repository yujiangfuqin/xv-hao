#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
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
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
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


  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}




#ifdef LAB_PGTBL
uint64
sys_pgaccess(void)
{ 
  uint64 va;
  int len;
  uint64 abits_addr;
  argaddr(0, &va);
  argint(1, &len);
  argaddr(2, &abits_addr);
  if(len < 0 || len > 32)
    return -1;
  
  uint32 ret = 0;
  pte_t *pte;
  struct proc *p = myproc();

  for(int i = 0; i < len; i++){
    if(va >= MAXVA)
      return -1;

    pte = walk(p->pagetable, va, 0);
    
    if(pte == 0)
      return -1;
    /* if pte has been accessed add bit of ret and clear*/
    if(*pte & PTE_A){
      ret |= (1 << i);
      *pte &= (~PTE_A);
    }
    /* va of next page */
    va += PGSIZE;    
  }

  if(copyout(p->pagetable, abits_addr, (char*)&ret, sizeof(ret)) < 0)
    return -1;
  return 0;
}
#endif


uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
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
