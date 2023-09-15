#include <stdlib.h>

#include "memory.h"

unsigned char* memoryCreate(void) {
    return malloc(MEMORY_SIZE);
}

void memoryDestroy(unsigned char* memory) {
    free(memory);
}
