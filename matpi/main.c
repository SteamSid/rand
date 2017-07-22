#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

// ############################################ DEBUG ############################################

#define _LM_DBG_FMT(_NUM,_MSG) case _NUM: \
  printf("[%#08x] %s %s\n",_LM_DBG_LVL,_MSG,msg); \
  break;

#define DLOG_NORMAL 0
#define DLOG_WARN   2
#define DLOG_ERROR  1
#define DLOG_CRIT   3

static int _LM_DBG_LVL=0;

uint32_t dlog(const char* msg,uint32_t ltp) {
  switch(ltp) {
    _LM_DBG_FMT(0,"");
    _LM_DBG_FMT(1,"\033[33;1mERROR:\033[0m");
    _LM_DBG_FMT(2,"\033[32;1mWARNING:\033[0m");
    _LM_DBG_FMT(3,"\033[31;1mCRITICAL:\033[0m");
    default:
      printf("Invalid debug mode\n");
      return 1;
      break; }
  _LM_DBG_LVL++;
  return 0; }

// ############################################ SCREEN ############################################

// ############################################ MAIN ############################################

int main(int argc,char *argv[]) {
  dlog("message",0);
  dlog("message",2);
  dlog("message",1);
  dlog("message",3);
  return 0; }
