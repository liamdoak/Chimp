#ifndef DISPLAY_H
#define DISPLAY_H

#define DISPLAY_WIDTH 64
#define DISPLAY_HEIGHT 32

unsigned char* displayCreate(void);
void displayDestroy(unsigned char* display);
unsigned char displayGetBit(unsigned char* display, unsigned char x,
    unsigned char y);
void displayXorBit(unsigned char* display, unsigned char bit, unsigned char x,
    unsigned char y);

#endif // DISPLAY_H
