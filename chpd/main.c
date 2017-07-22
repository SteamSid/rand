#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define IMODE_LENGTH 3

static const char str_imode[IMODE_LENGTH]={'b','x','d'};
static const char str_iflag[IMODE_LENGTH]={128,64 ,32 };

#undef IMODE_LENGTH
#undef str_imode
#undef str_flag

// ########################### DEBUGGING ###########################



// ########################### OPERATIONS ###########################

const char *op_n[]={
"cls","ret" ,"sys",
"jmp","call","se" ,
"ld" ,"add" ,"or" ,
"and","xor" ,"sub",
"shr","subn","shl",
"sne","jp"  ,"rnd",
"drw","skp" ,"skpn" };

uint16_t origin=0;
typedef const uint16_t (*op_rep) (int argc,char **argv);

const uint16_t op_cls (int argc,char *argv[]){
  if(argc>1){
    printf("CLS cannot have any arguments!\n");
    return 0; }
  return 0x00E0; }

const uint16_t op_ret(int argc,char *argv[]){
  if(argc>1){
    printf("RET cannot have any arguments!\n");
    return 0; }
  return 0x00EE; }

const uint16_t op_sys (int argc,char *argv[]){
  if(argc!=2){
    printf("SYS only uses one argument!\n");
    return 0; }
  return (strtoul(argv[1],NULL,0)&0b0000111111111111); }

const uint16_t op_jmp (int argc,char *argv[]){
  if(argc!=2){
    printf("JMP only uses one argument!\n");
    return 0; }
  return (strtoul(argv[1],NULL,0)&0b0000111111111111)|(1<<12); }

const uint16_t op_call(int argc,char *argv[]){
  return 0; }

const uint16_t op_se  (int argc,char *argv[]){
  return 0; }

const uint16_t op_ld  (int argc,char *argv[]){
  return 0; }

const uint16_t op_add (int argc,char *argv[]){
  return 0; }

const uint16_t op_or  (int argc,char *argv[]){
  return 0; }

const uint16_t op_and (int argc,char *argv[]){
  return 0; }

const uint16_t op_xor (int argc,char *argv[]){
  return 0; }

const uint16_t op_sub (int argc,char *argv[]){
  return 0; }

const uint16_t op_shr (int argc,char *argv[]){
  return 0; }

const uint16_t op_subn(int argc,char *argv[]){
  return 0; }

const uint16_t op_shl (int argc,char *argv[]){
  return 0; }

const uint16_t op_sne (int argc,char *argv[]){
  return 0; }

const uint16_t op_jp  (int argc,char *argv[]){
  return 0; }

const uint16_t op_rnd (int argc,char *argv[]){
  return 0; }

const uint16_t op_drw (int argc,char *argv[]){
  if(argc!=4){
    printf("DRW uses exactly 3 arguments!");
    return 0; }
  return (strtoul(argv[1],NULL,0)<<8)|
         (strtoul(argv[2],NULL,0)<<4)|
          strtoul(argv[3],NULL,0)|(0xD<<12); }

const uint16_t op_skp (int argc,char *argv[]){
  return 0; }

const uint16_t op_skpn(int argc,char *argv[]){
  return 0; }

const op_rep op_f[]={
  &op_cls,&op_ret ,&op_sys,
  &op_jp ,&op_call,&op_se ,
  &op_ld ,&op_add ,&op_or ,
  &op_and,&op_xor ,&op_sub,
  &op_shr,&op_subn,&op_shl,
  &op_sne,&op_rnd ,&op_drw,
  &op_skp,&op_skpn };

// ########################### TESTING ###########################

const uint16_t test_op_order[]={
  0,1,2
};

const uint16_t test_op_argc[]={
  0,0,
  1,1,1,
  2,2,2,2,2,
  2,2,2,2,2,2,2,2,2,
  2,
  1,1,
  2,
  3,
  1,1,
  1,1,1,1,1,1,1,1,1 };

// ########################### MAIN ###########################

int main(int argc,char *argv[]){
  printf("Startup\n");
  return 0; }
