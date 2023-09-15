#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE 4096

unsigned char* memoryCreate(void);
void memoryDestroy(unsigned char* memory);

#endif // MEMORY_H
