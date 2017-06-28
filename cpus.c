
######################## BEGIN DEBUG.H ########################

######################## END DEBUG.H ########################

######################## BEGIN CORE_CPU.H ########################

#ifndef H_CORE_CPU
#define H_CORE_CPU

#include <stdint.h>

struct cpu_handle {
  const char *name,*desc;
  uint32_t memsize;
  uint32_t stacksize;
  void (*init)  ();
  void (*cycle) ();
  void (*close) (); };

#endif

######################## END CORE_CPU.H ########################

######################## BEGIN CORE_CPU.C ########################

#include "core_cpu.h"
#include <stdio.h>

######################## END CORE_CPU.C ########################

######################## BEGIN MAIN.C ########################

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

int main(int argc,char *argv[]){
  printf("yeah\n");
  return 0;
}

######################## END MAIN.C ########################
