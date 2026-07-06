#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define SYS_MEM_CHECK 472

int main()
{
  unsigned long start_breakpoint;

  if (syscall(SYS_MEM_CHECK, &start_breakpoint))
    printf("error: %d\n", errno);


  unsigned long mid_breakpoint;

  int *vector1 = (int*) malloc(50000 * sizeof(int));
  for (int i = 0; i < 50000; i++) vector1[i] = i;
  
  free(vector1);

  if (syscall(SYS_MEM_CHECK, &mid_breakpoint))
    printf("error: %d\n", errno);

  if (mid_breakpoint > start_breakpoint)
    printf("Something went wrong, leak detected: %lu bytes\n", mid_breakpoint - start_breakpoint);
  else
    printf("All fine! Memory has been freed correctly\n");
  

  unsigned long finish_breakpoint;

  int *vector2 = (int*) malloc(50000 * sizeof(int));
  for (int i = 0; i < 50000; i++) vector2[i] = i;

  if (syscall(SYS_MEM_CHECK, &finish_breakpoint))
    printf("error: %d\n", errno);

  if (finish_breakpoint > mid_breakpoint)
    printf("Caution! %lu bytes leaked\n", finish_breakpoint - mid_breakpoint);
  else
    printf("Something went wrong, leak undetected\n");

  free(vector2);
  
  return 0;
}
