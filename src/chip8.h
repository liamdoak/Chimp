#ifndef CHIP8_H
#define CHIP8_H

#include <SDL2/SDL.h>

#define PROGRAM_START 0x200
#define CHIP8_CLOCK_SPEED 700
#define STACK_SIZE 16
#define FONT_START 0x30

typedef struct _Chip8 {
    // hardware
    unsigned char* memory;
    unsigned char* display;
    unsigned char* keyboard;

    // registers
    unsigned short i;
    unsigned char* v;
    unsigned short programCounter;
    unsigned char stackPointer;
    unsigned char delayTimer;
    unsigned char soundTimer;

    // extra
    char keyFlag;
} Chip8;

Chip8* chip8Create(void);
void chip8Destroy(Chip8* chip8);
unsigned char chip8LoadROM(Chip8* chip8, const char* romPath);
int chip8FDE(Chip8* chip8, unsigned char dbgFlag, unsigned char sFlag);
void chip8AssignSDLKey(Chip8* chip8, SDL_Keycode key, unsigned char state);

#endif // CHIP8_H
