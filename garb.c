#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// # Debugging # //

#define DLOG_LOG   0
#define DLOG_WARN  1
#define DLOG_ERROR 2
#define DLOG_CRIT  3

#define _DLOG_PRNT(YES) printf("[%#06x] %s %s\n",_dlog_depth,YES,str);

uint32_t _dlog_depth;

void dlog(const char* str,unsigned int lvl) {
  switch(lvl) {
    case DLOG_LOG: {
      _DLOG_PRNT("lnorm:");
      break; }
    case DLOG_WARN:  {
      _DLOG_PRNT("\033[33;1mlwarn:\033[0m");
      break; }
    case DLOG_ERROR: {
      _DLOG_PRNT("\033[31;1mlerrr:\033[0m");
      break; }
    case DLOG_CRIT:  {
      _DLOG_PRNT("\033[36;1mlcrit:\033[0m");
      break; } }
  _dlog_depth++; }

// # CPU # //

typedef void (*_op_ptr) (uint8_t);

const char* _op_nametable[256]={
  "BRK","ORA","NNN","NNN","NNN","ORA","ASL","NNN","PHP","ORA","ASL","NNN","NNN","ORA","ASL","NNN",
  "BPL","ORA","NNN","NNN","NNN","ORA","ASL","NNN","CLC","ORA","NNN","NNN","NNN","ORA","ASL","NNN",
  "JSR","AND","NNN","NNN","BIT","AND","ROL","NNN","PLP","AND","ROL","NNN","BIT","AND","ROL","NNN",
  "BMI","AND","NNN","NNN","NNN","AND","ROL","NNN","SEC","AND","NNN","NNN","NNN","AND","ROL","NNN",
  "RTI","EOR","NNN","NNN","NNN","EOR","LSR","NNN","PHA","EOR","LSR","NNN","JMP","EOR","LSR","NNN",
  "BVC","EOR","NNN","NNN","NNN","EOR","LSR","NNN","CLI","EOR","NNN","NNN","NNN","EOR","LSR","NNN",
  "RTS","ADC","NNN","NNN","NNN","ADC","ROR","NNN","PLA","ADC","ROR","NNN","JMP","ADC","ROR","NNN",
  "BVS","ADC","NNN","NNN","NNN","ADC","ROR","NNN","SEI","ADC","NNN","NNN","NNN","ADC","ROR","NNN",
  "NNN","STA","NNN","NNN","STY","STA","STX","NNN","DEY","NNN","TXA","NNN","STY","STA","STX","NNN",
  "BCC","STA","NNN","NNN","STY","STA","STX","NNN","TYA","STA","TXS","NNN","NNN","STA","NNN","NNN",
  "LDY","LDA","LDX","NNN","LDY","LDA","LDX","NNN","TAY","LDA","TAX","NNN","LDY","LDA","LDX","NNN",
  "BCS","LDA","NNN","NNN","LDY","LDA","LDX","NNN","CLV","LDA","TSX","NNN","LDY","LDA","LDX","NNN",
  "CPY","CMP","NNN","NNN","CPY","CMP","DEC","NNN","INY","CMP","DEX","NNN","CPY","CMP","DEC","NNN",
  "BNE","CMP","NNN","NNN","NNN","CMP","DEC","NNN","CLD","CMP","NNN","NNN","NNN","CMP","DEC","NNN",
  "CPX","SBC","NNN","NNN","CPX","SBC","INC","NNN","INX","SBC","NOP","NNN","CPX","SBC","INC","NNN",
  "BEQ","SBC","NNN","NNN","NNN","SBC","INC","NNN","SED","SBC","NNN","NNN","NNN","SBC","INC","NNN"};

_op_ptr _op_jumptable[256]={};

// # Main # //

int main(int argc,char *argv[]) {
  return 0; }