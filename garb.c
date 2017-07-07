#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define DLOG_LOG   0
#define DLOG_WARN  1
#define DLOG_ERROR 2
#define DLOG_CRIT  3

#define _DLOG_PRNT(YES) printf("[%#06x] %s %s",_dlog_depth,YES,str);

uint32_t _dlog_depth;

void dlog(const char* str,unsigned int lvl) {
  switch(lvl) {
    case DLOG_LOG: {
      _DLOG_PRNT("lnorm:"); }
    case DLOG_WARN:  {
      _DLOG_PRNT("\033[33;1mlwarn:\033[0m"); }
    case DLOG_ERROR: {
      _DLOG_PRNT("\033[31;1mlerrr:\033[0m"); }
    case DLOG_CRIT:  {
      _DLOG_PRNT("\033[36;1mlcrit:\033[0m"); }
  }
  _dlog_depth++; }
