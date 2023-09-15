#ifndef MAIN_H
#define MAIN_H

#define WINDOW_SCALE 8

typedef struct _Buffer {
    unsigned int* data;
    unsigned int width;
    unsigned int height;
    unsigned int scale;
} Buffer;

typedef struct _AudioData {
    float frequency;
    float time;
} AudioData;

Buffer* bufferCreate(unsigned int width, unsigned int height,
    unsigned int scale);
void bufferDestroy(Buffer* buffer);
void bufferUpdate(Buffer* buffer, Chip8* chip8);
void audioCallback(void* userData, unsigned char* stream, int len);

#endif // MAIN_H
