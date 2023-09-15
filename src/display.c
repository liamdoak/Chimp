#include <stdlib.h>

#include "display.h"

unsigned char* displayCreate(void) {
    return malloc(DISPLAY_WIDTH * DISPLAY_HEIGHT / 8);
}

void displayDestroy(unsigned char* display) {
    free(display);
}

unsigned char displayGetBit(unsigned char* display, unsigned char x,
    unsigned char y) {
    unsigned char xByte = x / 8;
    unsigned char xByteBit = x % 8;
    unsigned char andFactor = 0x80 >> xByteBit;
    unsigned char byte = display[y * (DISPLAY_WIDTH / 8) + xByte];
    unsigned char bit = byte & andFactor;
    return bit || 0;
}

void displayXorBit(unsigned char* display, unsigned char bit, unsigned char x,
    unsigned char y) {
    unsigned char xorFactor = bit << (7 - (x % 8));
    display[y * (DISPLAY_WIDTH / 8) + (x / 8)] ^= xorFactor;
}
