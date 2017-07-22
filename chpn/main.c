#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_image.h>

// ######################## TYPES ########################

typedef uint8_t  byte;
typedef uint16_t ushort;
typedef uint32_t uint32;
typedef void (*c8FuncPTR) (ushort);

// ######################## DEBUGGING ########################

uint32 LOG_DEPTH=0;

enum LOG_LEVEL {
  LOG_NORM, LOG_IMP,
  LOG_WARN, LOG_ERROR,
  LOG_CRIT, LOG_NOTE };

void dloge(const char* str,enum LOG_LEVEL loglvl) {
  switch(loglvl) {
    case LOG_NORM: {
      printf("[%#010x] %s\n",LOG_DEPTH,str);
      break; }
    case LOG_IMP:   {
      printf("\033[1m[%#010x]\033[0m %s\n",LOG_DEPTH,str);
      break; }
    case LOG_WARN:  {
      printf("\033[33;1m[%#010x] WARN:\033[0m %s\n",LOG_DEPTH,str);
      break; }
    case LOG_ERROR: {
      printf("\033[31;1m[%#010x] ERROR:\033[0m %s\n",LOG_DEPTH,str);
      break; }
    case LOG_CRIT:  {
      printf("\036[31;1m[%#010x] CRITICAL:\033[0m %s\n",LOG_DEPTH,str);
      break; }
    case LOG_NOTE:  {
      printf("\033[32;1m[%#010x] NOTE:\033[0m \n%s\n",LOG_DEPTH,str);
      break; }
  }
  ++LOG_DEPTH; }

void dlog(const char* str) {
  dloge(str,LOG_NORM); }

// ######################## STANDARD ########################

byte c8_font_standard[0x60]={
  0b01100000, 0b10010000, 0b11010000, 0b10110000, 0b01100000,    // 0
  0b00100000, 0b01100000, 0b00100000, 0b00100000, 0b01110000,    // 1
  0b11100000, 0b00010000, 0b01100000, 0b10000000, 0b11110000,    // 2
  0b11100000, 0b00010000, 0b11100000, 0b00010000, 0b11100000,    // 3
  0b10100000, 0b10100000, 0b11110000, 0b00100000, 0b00100000,    // 4
  0b11110000, 0b10000000, 0b01100000, 0b00010000, 0b11100000,    // 5
  0b01100000, 0b10000000, 0b11100000, 0b10010000, 0b01100000,    // 6
  0b11110000, 0b00010000, 0b00100000, 0b01000000, 0b01000000,    // 7
  0b01100000, 0b10010000, 0b01100000, 0b10010000, 0b01100000,    // 8
  0b01100000, 0b10010000, 0b01110000, 0b00010000, 0b01100000,    // 9
  0b01100000, 0b10010000, 0b10010000, 0b11110000, 0b10010000,    // A
  0b11100000, 0b10010000, 0b11100000, 0b10010000, 0b11100000,    // B
  0b01100000, 0b10010000, 0b10000000, 0b10010000, 0b01100000,    // C
  0b11100000, 0b10010000, 0b10010000, 0b10010000, 0b11100000,    // D
  0b11110000, 0b10000000, 0b11100000, 0b10000000, 0b11110000,    // E
  0b11110000, 0b10000000, 0b11100000, 0b10000000, 0b10000000, }; // F

char c8_keymap_qwerty [0x10]={
  'x',             // 0
  '1','2','3',     // 1-3
  'q','w','e',     // 4-6
  'a','s','d',     // 7-9
  'z','c','4',     // A,B,C
  'r','f','v'  };  // D,E,F

char c8_keymap_dvorak [0X10]={
  'q',             // 0
  '1' ,'2','3',    // 1-3
  '\'',',','.',    // 4-6
  'a' ,'o','e',    // 7-9
  ';' ,'j','4',    // A,B,C
  'p' ,'u','k'  }; // D,E,F

uint8_t c8_keymap_dvorak_sdl [0x10]={
  SDLK_q,
  SDLK_1        ,SDLK_2,SDLK_3,
  SDLK_QUOTE    ,SDLK_COMMA   ,SDLK_PERIOD,
  SDLK_a        ,SDLK_o       ,SDLK_e,
  SDLK_SEMICOLON,SDLK_j       ,SDLK_4,
  SDLK_p        ,SDLK_u       ,SDLK_k };

// ######################## CORE ########################

byte*   c8Mem;
byte*   c8Var;
byte*   c8Scr;
char*   c8KeyMap;
byte*   c8KeyPressMap;
ushort* c8Stack;

ushort PC,I;
byte   SP,ST,DT;

int c8Load     (const char* path) {
  FILE* file = fopen(path,"r");
  if(!file) {
    printf("Unable to read file %s\n",path); return -1; }
  if(!c8Mem)  {
    printf("Memory not allocated!\n"); return -2; }

  struct stat info;
  stat(path,&info);

  fread(c8Mem+0x200,info.st_size,1,file);
  fclose(file);
  return 0;  }

void c8SetFont  (byte* font) {
  memcpy(c8Mem,font,0x60); }

void c8SetKeyMap(char* keyMap) {
  if(c8KeyMap==NULL) c8KeyMap=(char*)malloc(16);
  memcpy(c8KeyMap,keyMap,16); }

void c8Init(byte* font,char* keyMap) {
  PC=0x200;
  I=0;
  SP=0;
  ST=0;
  DT=0;

  c8Mem=(byte*)malloc(0x10000);
  c8Var=(byte*)malloc(16);
  c8Scr=(byte*)malloc(32*64);

  c8KeyPressMap=(byte*)malloc(16);

  c8Stack=(ushort*)malloc(16*sizeof(ushort));

  memset(c8Mem,0,0x10000);
  memset(c8Var,0,16);
  memset(c8Scr,0,32*64);
  memset(c8Stack,0,16*sizeof(ushort));
  memset(c8KeyPressMap,0,16);

  c8SetFont  (font);
  c8SetKeyMap(keyMap); }

void c8Quit() {
  free(c8Mem);
  free(c8Stack);
  free(c8Var);
  free(c8Scr);
  free(c8KeyMap); }

bool c8draw=0;

#define C8_OP    (ushort)(((ushort)c8Mem[PC]<<8)|((ushort)c8Mem[PC+1]))
#define C8_OP_N3    (OP                    >>12)
#define C8_OP_N2   ((OP&0b0000111100000000)>>8 )
#define C8_OP_N1   ((OP&0b0000000011110000)>>4 )
#define C8_OP_N0    (OP&0b0000000000001111)
#define C8_OP_B1    (OP                    >>8 )
#define C8_OP_B0    (OP&0b0000000011111111)
#define C8_OP_ADDR  (OP&0b0000111111111111)

void  c8OP_SYSADDR(ushort OP) { // 0NNN
  dlog("[C8] SYSADDR Called");
  PC+=2;             } // Unused
void c8SET_0      (ushort OP) {
  switch(OP) {
    case 0x00E0: {
      c8draw=true;
      memset(c8Scr,0,64*32);
      PC+=2;
      break; } // Clear Screen
    case 0x00EE: {
      PC=c8Stack[SP-=1]+2;
      break; } // Return from Subroutine
    default:
      dloge("[C8] Unknown Instruction!",LOG_IMP);
      printf("             [C8] %#06x\n",OP);
      break; } } // Command set 0x0

void  c8OP_JMPADDR(ushort OP) { // 1NNN
  PC=C8_OP_ADDR; } // JuMP to ADDRess
void  c8OP_CLLADDR(ushort OP) { // 2NNN
  c8Stack[SP]=PC;
  SP+=1;
  PC=C8_OP_ADDR; } // CaLL ADDRess

#define _C8_COMP_VKK c8Var[C8_OP_N2]==C8_OP_B0

void  c8OP_SIFXEKK(ushort OP) { // 3XKK
  if(_C8_COMP_VKK) {
    PC+=4;
  } else {
    PC+=2; } } // Skip instruction IF X is Equal to KK
void  c8OP_SNOXEKK(ushort OP) { // 4XKK
  if(!_C8_COMP_VKK) {
    PC+=4;
  } else {
    PC+=2; } } // Skip instruction if X is NOt Equal to KK

#undef _C8_COMP_VKK

void  c8OP_SIFXEQY(ushort OP) { // 5XY0
  if(c8Var[C8_OP_N2]==c8Var[C8_OP_N1]) {
    PC+=4;
  } else {
    PC+=2; } } // Skip instruction IF X is EQual to Y

void  c8OP_PUKKINX(ushort OP) { // 6XKK
  c8Var[C8_OP_N2]=C8_OP_B0;
  PC+=2; } // PUt KK INto X

void  c8OP_ADKKTOX(ushort OP) { // 7XKK
  int v=((int)c8Var[C8_OP_N2])+C8_OP_B0;
  if(v>255) {
    v-=256; }
  c8Var[C8_OP_N2]=v;
  PC+=2; } // ADd KK TO X

#define _C8_BIT_MATH(MODE) c8Var[C8_OP_N2] MODE c8Var[C8_OP_N1]

void c8SET_8      (ushort OP) {
  switch(C8_OP_N0) {
    case 0x0: { // 8XY0
      c8Var[C8_OP_N2]=c8Var[C8_OP_N1];
      PC+=2;
      break; } // Put Y into X
    case 0x1: { // 8XY1
      c8Var[C8_OP_N2]=_C8_BIT_MATH(|);
      PC+=2;
      break; } // X Equals X OR Y
    case 0x2: { // 8XY2
      c8Var[C8_OP_N2]=_C8_BIT_MATH(&);
      PC+=2;
      break; } // X Equals X AND Y
    case 0x3: { // 8XY3
      c8Var[C8_OP_N2]=_C8_BIT_MATH(^);
#undef _C8_BIT_MATH
      PC+=2;
      break; } // X Equals X XOR Y
    case 0x4: { // 8XY4
      c8Var[0xF]=(c8Var[C8_OP_N1]>c8Var[C8_OP_N2]);
      c8Var[C8_OP_N2]=c8Var[C8_OP_N2]+c8Var[C8_OP_N1];
      PC+=2;
      break; } // X Equals X + Y With Carry
    case 0x5: { // 8XY5
      c8Var[0xF]=(c8Var[C8_OP_N2]>c8Var[C8_OP_N1]);
      c8Var[C8_OP_N2]=c8Var[C8_OP_N2]-c8Var[C8_OP_N1];
      PC+=2;
      break; } // X Equals X - Y NOT Borrow
    case 0x6: { // 8XY6
      c8Var[0xF]=c8Var[C8_OP_N1]&0b00000001;
      c8Var[C8_OP_N2]=c8Var[C8_OP_N1]>>1;
      PC+=2;
      break; } // Right Shift X
    case 0x7: { // 8XY7
      c8Var[0xF]=(c8Var[C8_OP_N2]>c8Var[C8_OP_N1]);
      c8Var[C8_OP_N2]=c8Var[C8_OP_N1]-c8Var[C8_OP_N2];
      PC+=2;
      break; } // X Equals Y - X NOT Borrow
    case 0xE: { // 8XYE
      c8Var[0xF]=((c8Var[C8_OP_N1]&0b10000000)>>7);
      c8Var[C8_OP_N2]=c8Var[C8_OP_N1]<<1;
      PC+=2;
      break; } // Left Shift X
    default:
    dloge("[C8] Unknown Instruction!",LOG_IMP);
    printf("             [C8] %#06x\n",OP);
    break; } } // Command set 0x8

void  c8OP_SIFXNOY(ushort OP) { // 9XY0
  if(c8Var[C8_OP_N2]!=c8Var[C8_OP_N1]) {
    PC+=4;
  } else {
    PC+=2; } } // Skip instruction IF X is NOt Y

void  c8OP_IEQADDR(ushort OP) { // ANNN
  I=C8_OP_ADDR;
  PC+=2; } // I EQuals ADDR

void  c8OP_JMPADV0(ushort OP) { // BNNN
  PC=C8_OP_ADDR+c8Var[0]; } // JuMP to ADDR + V[0]

void  c8OP_ANRKKIX(ushort OP) { // CXKK
  c8Var[C8_OP_N2]=C8_OP_B0&(rand()%255);
  PC+=2; } // Put Random ANd KK Into X

void  c8OP_DRSPAXY(ushort OP) { // DXYN
  c8draw=1;
  c8Var[0xF]=0;
  int set=0;
  for(int i=0; i<C8_OP_N0; ++i) {
    for(int b=0; b<8; ++b) {
      int pix=((c8Mem[I+i]>>(7-b))&0b00000001);
      if(pix) {
        int pos=(b+c8Var[C8_OP_N2]+((c8Var[C8_OP_N1]+i)*64));
        int pre=c8Scr[pos];
        c8Scr[pos]^=1;
        if(!set) {
          if(pre&&pix) {
            c8Var[0xF]=1;
            set=1;
          }
        }
      }
    }
  }
  PC+=2; } // DRaw SPrite At X Y sized by N

void  c8SET_E     (ushort OP) {
  switch(C8_OP_B0) {
    case 0x9E: { // EX9E
      if(c8KeyPressMap[c8Var[C8_OP_N2]]==1) {
        PC+=4; } else {
        PC+=2; }
      break; } // Skip instruction if key pressed
    case 0xA1: { // EXA1
      if(c8KeyPressMap[c8Var[C8_OP_N2]]==0) {
        PC+=4; } else {
        PC+=2; }
      break; } // Skip instruction if key is not pressed
    default:
      dloge("[C8] Unknown Instruction!",LOG_IMP);
      printf("             [C8] %#06x\n",OP);
      break; } } // Command set E

void c8SET_F      (ushort OP) {
  switch(C8_OP_B0) {
    case 0x07: { // FX07
      c8Var[C8_OP_N2]=DT;
      PC+=2;
      break; } // X Equals DT
    case 0x0A: { // FX0A
      for(int i = 0; i < 16; ++i) {
        if(c8KeyPressMap[i]==1) {
          PC+=2;
          c8Var[C8_OP_N2]=i;
          break; } }
      break; } // Stop until key pressed
    case 0x15: { // FX15
      DT=c8Var[C8_OP_N2];
      PC+=2;
      break; } // Set DT to V[x]
    case 0x18: { // FX18
      ST=c8Var[C8_OP_N2];
      PC+=2;
      break; } // Set ST to V[x]
    case 0x1E: { // FX1E
      I=I+c8Var[C8_OP_N2];
      PC+=2;
      break; } // Set I to I + V[x]
    case 0x29: { // FX29
      I=c8Var[C8_OP_N2]*5;
      PC+=2;
      break; } // Set I to the sprite font location
    case 0x33: { // FX33
      c8Mem[I]   = (c8Var[C8_OP_N2]/100)%10;
      c8Mem[I+1] = (c8Var[C8_OP_N2]/10)%10;
      c8Mem[I+2] =  c8Var[C8_OP_N2]%10;
      PC+=2;
      break; } // Store BCD representation of number into I, I + 1, and I + 2
    case 0x55: { // FX55
      for(int a = 0; a <= C8_OP_N2; ++a) {
        c8Mem[I+a] = c8Var[a]; }
      I++;
      PC+=2;
      break; } // Store V[0] - V[x] in mem starting at I
    case 0x65: { // FX65
      for(int a = 0; a <= C8_OP_N2; ++a) {
        c8Var[a] = c8Mem[I+a]; }
      I++;
      PC+=2;
      break; } // Read from mem starting at I to V[0] to V[x]
  } }

c8FuncPTR c8FuncJumpTable[0x10]={
  &c8SET_0,
  &c8OP_JMPADDR,&c8OP_CLLADDR,
  &c8OP_SIFXEKK,&c8OP_SNOXEKK,&c8OP_SIFXEQY,
  &c8OP_PUKKINX,&c8OP_ADKKTOX,
  &c8SET_8,
  &c8OP_SIFXNOY,
  &c8OP_IEQADDR,
  &c8OP_JMPADV0,
  &c8OP_ANRKKIX,
  &c8OP_DRSPAXY,
  &c8SET_E,
  &c8SET_F };

void c8Step() {
  ushort OP=C8_OP;
  c8FuncJumpTable[C8_OP_N3] (OP); }

// ######################## MAIN ########################

int main(int argc, char *argv[]) {
  dlog("Startup");

  int scrmulti =8;

  dlog("[SDL2] Initializing...");
  dlog("[SDL2] Initializing Everything");
  SDL_Init(SDL_INIT_EVERYTHING);
  dlog("[SDL2] Initializing SDL2_image");
  IMG_Init(IMG_INIT_PNG);

  SDL_Window   *win;
  SDL_Surface  *winsurf;
  SDL_Renderer *ren;

  dlog("[SDL2] Creating Window...");
  win=SDL_CreateWindow("Chippin'",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,64*scrmulti,32*scrmulti,0);
  if(win==NULL) { dloge("[SDL2] Failed to create window!",LOG_CRIT); return -1; }

  dlog("[SDL2] Getting Window Surface...");
  winsurf=SDL_GetWindowSurface(win);

  dlog("[SDL2] Creating Software Renderer...");
  ren=SDL_CreateSoftwareRenderer(winsurf);
  if(ren==NULL) { dloge("[SDL2] Failed to create renderer!",LOG_CRIT); return -1; }

  dlog("[SDL2] Gathering Renderer Info...");
  SDL_RendererInfo rinfo;
  SDL_GetRendererInfo(ren,&rinfo);
  printf("             [SDL2] Renderer: %s\n             [SDL2] Formats:\n",rinfo.name);
  for(int i = 0; i < rinfo.num_texture_formats; ++i) {
    printf("             [SDL2] %s\n",SDL_GetPixelFormatName(rinfo.texture_formats[i])); }

  const unsigned int scrwidth=64,scrheight=32;
  dlog("Creating screen texture");
  SDL_Texture* scrtex=SDL_CreateTexture(ren,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,scrwidth,scrheight);
  dlog("Allocating screen memory");
  byte* scrmem=(byte*)malloc(scrwidth*scrheight*4);

  SDL_Rect scrrect={ .w=64*scrmulti,.h=32*scrmulti,.x=0,.y=0 };

  dlog("[C8] Initializing...");
  c8Init(c8_font_standard,c8_keymap_dvorak);
  dlog("[C8] Loading file...");
  c8Load(argv[1]);

  /* SDL_Texture* ramtex=SDL_CreateTexture(ren,SDL_PIXELFORMAT_ARGB8888,SDL_TEXTUREACCESS_STREAMING,8,4096);
  byte* rtxmem=(byte*)malloc(8*4096*4);
  memset(rtxmem,0,8*4096*4);
  for(int i = 0; i < 0x1000; ++i) {
    for(int b = 0; b < 8; ++b) {
      int val=(c8Mem[i]>>(8-b))&0b00000001;
      if( val) printf("1");
      if(!val) printf("0");
      if(b==7) printf("\n");
      if(val==1) {
        rtxmem[(b*4)  +(i*8*4)] = 255;
        rtxmem[(b*4)+1+(i*8*4)] = 255;
        rtxmem[(b*4)+2+(i*8*4)] = 255;
        rtxmem[(b*4)+3+(i*8*4)] = SDL_ALPHA_OPAQUE; } } }
  SDL_UpdateTexture(ramtex,NULL,rtxmem,8*4);

  SDL_Rect ramrect={ .w=32,.h=16384,.x=64*scrmulti,.y=0 }; */

  /* for(int i = 0; i < 0x10000; ++i) {
    printf("%#04x ",c8Mem[i]); }
  return 0; */

  dlog("Startup Successful");

  bool run=true;
  SDL_Event e;
  int frame;
  while(run) {
    // printf("\x1b[ACURR: PC: %#06x MM0: %#04x MM1: %#04x I: %#06x DT: %#04x ST: %#04x \n",PC,c8Mem[PC],c8Mem[PC+1],I,DT,ST);
    // printf("NEXT: %#06x MM0: %#04x MM1: %#04x \n",PC+2,c8Mem[PC+2],c8Mem[PC+3]);
    if(frame%16==0) {
      if(DT>0) --DT;
      if(ST>0) --ST; }
      SDL_Delay(1);
    c8Step();
    while(SDL_PollEvent(&e)) {
      // ramrect.y-=1;
      switch(e.type) {
        case SDL_QUIT:
          run=false;
          break;
        case SDL_KEYUP:
          for(int i = 0; i < 0x10; ++i) {
            if(e.key.keysym.sym==c8_keymap_dvorak_sdl[i]) {
              c8KeyPressMap[i]=0; } }
          break;
        case SDL_KEYDOWN:
          for(int i = 0; i < 0x10; ++i) {
            if(e.key.keysym.sym==c8_keymap_dvorak_sdl[i]) {
              c8KeyPressMap[i]=1; } }
          break;
      }
    }
    if(c8draw) {
      c8draw=0;
      for(unsigned int i = 0; i < 64*32; ++i) {
        if(c8Scr[i]==0) {
          scrmem[(i*4)  ] = 0;
          scrmem[(i*4)+1] = 0;
          scrmem[(i*4)+2] = 0;
          scrmem[(i*4)+3] = SDL_ALPHA_OPAQUE;
        } else {
          scrmem[(i*4)  ] = 255;
          scrmem[(i*4)+1] = 255;
          scrmem[(i*4)+2] = 255;
          scrmem[(i*4)+3] = SDL_ALPHA_OPAQUE;
      } }

      SDL_UpdateTexture(scrtex,NULL,scrmem,scrwidth*4);

      SDL_SetRenderDrawColor(ren,255,255,255,255);
      SDL_RenderClear(ren);

      SDL_RenderCopy(ren,scrtex,NULL,&scrrect);
      // SDL_RenderCopy(ren,ramtex,NULL,&ramrect);

      // SDL_RenderPresent(ren);
      SDL_UpdateWindowSurface(win); }
    frame+=1;
  }

  dlog("Quitting...");
  dlog("[C8] Shutting down");
  c8Quit();
  dlog("Deallocating screen memory");
  free(scrmem);
  dlog("Destroying screen texture");
  SDL_DestroyTexture(scrtex);
  dlog("[SDL2] Shutting down...");
  dlog("[SDL2] Destroying Renderer");
  SDL_DestroyRenderer(ren);
  dlog("[SDL2] Destroying Window");
  SDL_DestroyWindow(win);

  dlog("[SDL2] Quitting SDL2_image");
  IMG_Quit();
  dlog("[SDL2] Quitting");
  SDL_Quit();
  dlog("Graceful Shutdown");
  return 0; }
