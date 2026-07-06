#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/uaccess.h>


SYSCALL_DEFINE1(mem_check, unsigned long __user *, allocated)
{
  unsigned long heap_size = 0;

  if (!current || !current->mm)
    return -EINVAL;
  
  if (current->mm->brk > current->mm->start_brk)
    heap_size = current->mm->brk - current->mm->start_brk;
  
  if (copy_to_user(allocated, &heap_size, sizeof(unsigned long)))
    return -EFAULT;

  return 0;
}