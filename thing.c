#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>

#define MEM_SIZE 1048576

uint8_t *mem;

struct screen{
  uint16_t w,h;
  SDL_Surface* surf; };
  
// CHAR MEM
// 6*8*2 Per character font
// 6*8 Pixels *2 Bytes per pixel
// 0xC300 - 0xF300

// SPRITE MEM
// 0x8000 - 0x8100 - 2 Byte RGBA color table
// 0x8100 - 0x8300 - SPRITE TABLE - 8 Bytes each
//    SPRITE TABLE
//    B1    = X
//    B2    = Y
//    B3    = Attributes
//    B4/B5 = Pointer to color table
//    B5/B6 = Pointer to sprite data
//    B7    = Sprite data width
//    B8    = Sprite data height
// 0x8300 - 0xC300 - 4KB screen info - 2 bytes per pixel

void gfx_render(screen* scr) {
  
}

int main(int argc,char *argv[]){
  mem=(uint8_t*)malloc(MEM_SIZE);
  return 0; }
