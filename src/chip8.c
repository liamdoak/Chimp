#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#include "chip8.h"
#include "display.h"
#include "memory.h"

Chip8* chip8Create(void) {
    Chip8* chip8 = malloc(sizeof(Chip8));
    chip8->memory = memoryCreate();
    chip8->display = displayCreate();
    chip8->keyboard = malloc(sizeof(unsigned char) * 16);
    chip8->i = 0;
    chip8->delayTimer = 0;
    chip8->soundTimer = 0;

    chip8->v = malloc(sizeof(unsigned char) * 16);

    for(int i = 0; i < 16; i++) {
        chip8->keyboard[i] = 0;
        chip8->v[i] = 0;
    }

    chip8->keyFlag = -1;

    // load font
    // 0
    chip8->memory[FONT_START] = 0xF0;
    chip8->memory[FONT_START + 1] = 0x90;
    chip8->memory[FONT_START + 2] = 0x90;
    chip8->memory[FONT_START + 3] = 0x90;
    chip8->memory[FONT_START + 4] = 0xF0;

    // 1
    chip8->memory[FONT_START + 5] = 0x20;
    chip8->memory[FONT_START + 6] = 0x60;
    chip8->memory[FONT_START + 7] = 0x20;
    chip8->memory[FONT_START + 8] = 0x20;
    chip8->memory[FONT_START + 9] = 0x70;

    // 2
    chip8->memory[FONT_START + 10] = 0xF0;
    chip8->memory[FONT_START + 11] = 0x10;
    chip8->memory[FONT_START + 12] = 0xF0;
    chip8->memory[FONT_START + 13] = 0x80;
    chip8->memory[FONT_START + 14] = 0xF0;

    // 3
    chip8->memory[FONT_START + 15] = 0xF0;
    chip8->memory[FONT_START + 16] = 0x10;
    chip8->memory[FONT_START + 17] = 0xF0;
    chip8->memory[FONT_START + 18] = 0x10;
    chip8->memory[FONT_START + 19] = 0xF0;

    // 4
    chip8->memory[FONT_START + 20] = 0x90;
    chip8->memory[FONT_START + 21] = 0x90;
    chip8->memory[FONT_START + 22] = 0xF0;
    chip8->memory[FONT_START + 23] = 0x10;
    chip8->memory[FONT_START + 24] = 0x10;

    // 5
    chip8->memory[FONT_START + 25] = 0xF0;
    chip8->memory[FONT_START + 26] = 0x80;
    chip8->memory[FONT_START + 27] = 0xF0;
    chip8->memory[FONT_START + 28] = 0x10;
    chip8->memory[FONT_START + 29] = 0xF0;

    // 6
    chip8->memory[FONT_START + 30] = 0xF0;
    chip8->memory[FONT_START + 31] = 0x80;
    chip8->memory[FONT_START + 32] = 0xF0;
    chip8->memory[FONT_START + 33] = 0x90;
    chip8->memory[FONT_START + 34] = 0xF0;

    // 7
    chip8->memory[FONT_START + 35] = 0xF0;
    chip8->memory[FONT_START + 36] = 0x10;
    chip8->memory[FONT_START + 37] = 0x20;
    chip8->memory[FONT_START + 38] = 0x40;
    chip8->memory[FONT_START + 39] = 0x40;

    // 8
    chip8->memory[FONT_START + 40] = 0xF0;
    chip8->memory[FONT_START + 41] = 0x90;
    chip8->memory[FONT_START + 42] = 0xF0;
    chip8->memory[FONT_START + 43] = 0x90;
    chip8->memory[FONT_START + 44] = 0xF0;

    // 9
    chip8->memory[FONT_START + 45] = 0xF0;
    chip8->memory[FONT_START + 46] = 0x90;
    chip8->memory[FONT_START + 47] = 0xF0;
    chip8->memory[FONT_START + 48] = 0x10;
    chip8->memory[FONT_START + 49] = 0xF0;

    // A
    chip8->memory[FONT_START + 50] = 0xF0;
    chip8->memory[FONT_START + 51] = 0x90;
    chip8->memory[FONT_START + 52] = 0xF0;
    chip8->memory[FONT_START + 53] = 0x90;
    chip8->memory[FONT_START + 54] = 0x90;

    // B
    chip8->memory[FONT_START + 55] = 0xE0;
    chip8->memory[FONT_START + 56] = 0x90;
    chip8->memory[FONT_START + 57] = 0xE0;
    chip8->memory[FONT_START + 58] = 0x90;
    chip8->memory[FONT_START + 59] = 0xE0;

    // C
    chip8->memory[FONT_START + 60] = 0xF0;
    chip8->memory[FONT_START + 61] = 0x80;
    chip8->memory[FONT_START + 62] = 0x80;
    chip8->memory[FONT_START + 63] = 0x80;
    chip8->memory[FONT_START + 64] = 0xF0;

    // D
    chip8->memory[FONT_START + 65] = 0xE0;
    chip8->memory[FONT_START + 66] = 0x90;
    chip8->memory[FONT_START + 67] = 0x90;
    chip8->memory[FONT_START + 68] = 0x90;
    chip8->memory[FONT_START + 69] = 0xE0;

    // E
    chip8->memory[FONT_START + 70] = 0xF0;
    chip8->memory[FONT_START + 71] = 0x80;
    chip8->memory[FONT_START + 72] = 0xF0;
    chip8->memory[FONT_START + 73] = 0x80;
    chip8->memory[FONT_START + 74] = 0xF0;

    // F
    chip8->memory[FONT_START + 75] = 0xF0;
    chip8->memory[FONT_START + 76] = 0x80;
    chip8->memory[FONT_START + 77] = 0xF0;
    chip8->memory[FONT_START + 78] = 0x80;
    chip8->memory[FONT_START + 79] = 0x80;

    return chip8;
}

void chip8Destroy(Chip8* chip8) {
    memoryDestroy(chip8->memory);
    displayDestroy(chip8->display);
    free(chip8->keyboard);
    free(chip8->v);
    free(chip8);
}

unsigned char chip8LoadROM(Chip8* chip8, const char* romPath) {
    FILE* rom = fopen(romPath, "rb");

    if(!rom)
        return 0;

    fseek(rom, 0, SEEK_END);
    fpos_t fileSize = ftell(rom);
    rewind(rom);

    for(int i = 0; i < fileSize; i++) {
        chip8->memory[PROGRAM_START + i] = fgetc(rom);
    }

    fclose(rom);

    chip8->programCounter = PROGRAM_START;

    return 1;
}

int chip8FDE(Chip8* chip8, unsigned char dbgFlag, unsigned char sFlag) {
    // fetch instruction
    unsigned short msb = chip8->memory[chip8->programCounter++];
    unsigned short lsb = chip8->memory[chip8->programCounter++];
    unsigned short instruction = msb << 8 | lsb;

    if(dbgFlag)
        printf("%.4X  %.4X\n", chip8->programCounter - 2, instruction);

    // decode and execute
    switch(instruction & 0xF000) {
        case 0x0000: {
            switch(instruction & 0x00FF) {
                case 0x00E0: {
                    // clear screen
                    for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8;
                        i++) {
                        chip8->display[i] = 0x00;
                    }
                    break;
                }

                case 0x00EE: {
                    // return
                    unsigned char lsb = chip8->memory[--chip8->stackPointer];
                    unsigned char msb = chip8->memory[--chip8->stackPointer];
                    unsigned short retAddr = msb << 8 | lsb;
                    chip8->programCounter = retAddr;
                    break;
                }

                default: {
                    return instruction;
                }
            }
            break;
        }

        case 0x1000: {
            // jump
            chip8->programCounter = instruction & 0x0FFF;
            break;
        }

        case 0x2000: {
            // go to sub-routine
            chip8->memory[chip8->stackPointer++] = chip8->programCounter >> 8;
            chip8->memory[chip8->stackPointer++] = chip8->programCounter & 0xFF;
            chip8->programCounter = instruction & 0x0FFF;
            break;
        }

        case 0x3000: {
            // skip next if equal
            unsigned char reg = (instruction & 0x0F00) >> 8;
            if((instruction & 0x00FF) == chip8->v[reg])
                chip8->programCounter += 2;
            break;
        }

        case 0x4000: {
            // skip next if not equal
            unsigned char reg = (instruction & 0x0F00) >> 8;
            if((instruction & 0x00FF) != chip8->v[reg])
                chip8->programCounter += 2;
            break;
        }

        case 0x5000: {
            // skip next if X equals Y
            unsigned char vx = (instruction & 0x0F00) >> 8;
            unsigned char vy = (instruction & 0x00F0) >> 4;
            if(chip8->v[vx] == chip8->v[vy])
                chip8->programCounter += 2;
            break;
        }

        case 0x6000: {
            // set register
            unsigned char reg = (instruction & 0x0F00) >> 8;
            chip8->v[reg] = instruction & 0x00FF;
            break;
        }

        case 0x7000: {
            // add to register
            unsigned char reg = (instruction & 0x0F00) >> 8;
            chip8->v[reg] += instruction & 0x00FF;
            break;
        }

        case 0x8000: {
            switch(instruction & 0x000F) {
                case 0x0000: {
                    // set Y to X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    chip8->v[vx] = chip8->v[vy];
                    break;
                }

                case 0x0001: {
                    // or X and Y and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    chip8->v[vx] |= chip8->v[vy];
                    break;
                }

                case 0x0002: {
                    // and X and Y and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    chip8->v[vx] &= chip8->v[vy];
                    break;
                }

                case 0x0003: {
                    // xor X and Y and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    chip8->v[vx] ^= chip8->v[vy];
                    break;
                }

                case 0x0004: {
                    // add X and Y and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    unsigned short sum = chip8->v[vx] + chip8->v[vy];

                    if(sum > 255)
                        chip8->v[0xF] = 1;
                    else
                        chip8->v[0xF] = 0;

                    chip8->v[vx] = (unsigned char) sum;
                    break;
                }

                case 0x0005: {
                    // sub X and Y (X - Y) and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    unsigned short diff = chip8->v[vx] - chip8->v[vy];

                    if(chip8->v[vx] > chip8->v[vy])
                        chip8->v[0xF] = 1;
                    else
                        chip8->v[0xF] = 0;

                    chip8->v[vx] = diff;
                    break;
                }

                case 0x0006: {
                    // shift right Y by 1 and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;

                    if(!sFlag) {
                        chip8->v[vx] = chip8->v[vy];
                    }

                    chip8->v[0xF] = chip8->v[vx] & 0x01;
                    chip8->v[vx] = chip8->v[vx] >> 1;
                    break;
                }

                case 0x0007: {
                    // sub Y and X (Y - X) and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;
                    unsigned short diff = chip8->v[vy] - chip8->v[vx];

                    if(chip8->v[vy] > chip8->v[vx])
                        chip8->v[0xF] = 1;
                    else
                        chip8->v[0xF] = 0;

                    chip8->v[vx] = diff;
                    break;
                }

                case 0x000E: {
                    // shift left Y by 1 and store in X
                    unsigned char vx = (instruction & 0x0F00) >> 8;
                    unsigned char vy = (instruction & 0x00F0) >> 4;

                    if(!sFlag) {
                        chip8->v[vx] = chip8->v[vy];
                    }

                    chip8->v[0xF] = (chip8->v[vx] & 0x80) >> 7;
                    chip8->v[vx] = chip8->v[vx] << 1;
                    break;
                }

                default: {
                    return instruction;
                }
            }
            break;
        }

        case 0x9000: {
            // skip next if X does not equal Y
            unsigned char vx = (instruction & 0x0F00) >> 8;
            unsigned char vy = (instruction & 0x00F0) >> 4;
            if(chip8->v[vx] != chip8->v[vy])
                chip8->programCounter += 2;
            break;
        }

        case 0xA000: {
            // set i register
            chip8->i = instruction & 0x0FFF;
            break;
        }

        case 0xB000: {
            // jump with offset
            chip8->programCounter = instruction & 0x0FFF + chip8->v[0x0];
            break;
        }

        case 0xC000: {
            // generate a random number, and AND it with kk and store in X
            unsigned char reg = (instruction & 0x0F00) >> 8;
            unsigned char andFactor = instruction & 0x00FF;
            chip8->v[reg] = rand() & andFactor;
            break;
        }

        case 0xD000: {
            // draw sprite
            unsigned char vx = (instruction & 0x0F00) >> 8;
            unsigned char x = chip8->v[vx] % 64;
            unsigned char vy = (instruction & 0x00F0) >> 4;
            unsigned char y = chip8->v[vy] % 32;
            unsigned char n = (instruction & 0x000F);
            chip8->v[0xF] = 0;

            for(int in = 0; in < n; in++) {
                unsigned char byte = chip8->memory[chip8->i + in];

                for(int ib = 0; ib < 8; ib++) {
                    unsigned char andFactor = 0x80 >> ib;
                    unsigned char bit = byte & andFactor;

                    if(x + ib > 63) {
                        break;
                    }

                    if(bit && displayGetBit(chip8->display, x + ib, y + in))
                        chip8->v[0xF] = 1;

                    displayXorBit(chip8->display, bit || 0, x + ib, y + in);
                }

                if(y + in > 31) {
                    break;
                }
            }

            break;
        }

        case 0xE000: {
            switch(instruction & 0x00FF) {
                case 0x009E: {
                    // skip next if X is pressed
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    if(chip8->keyboard[chip8->v[reg]])
                        chip8->programCounter += 2;
                    break;
                }

                case 0x00A1: {
                    // skip next if X is not pressed
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    if(!chip8->keyboard[chip8->v[reg]])
                        chip8->programCounter += 2;
                    break;
                }

                default: {
                    return instruction;
                }
            }
            break;
        }

        case 0xF000: {
            switch(instruction & 0x00FF) {
                case 0x0007: {
                    // store delay timer in X
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    chip8->v[reg] = chip8->delayTimer;
                    break;
                }

                case 0x000A: {
                    // Stop running instrucitons until a key is pressed and
                    // store the key in X
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    if(chip8->keyFlag == -1)
                        chip8->programCounter -= 2;
                    else
                        chip8->v[reg] = chip8->keyFlag;
                    break;
                }

                case 0x0015: {
                    // store X in the delay timer
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    chip8->delayTimer = chip8->v[reg];
                    break;
                }

                case 0x0018: {
                    // store X in the sound timer
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    chip8->soundTimer = chip8->v[reg];
                    break;
                }

                case 0x001E: {
                    // add X to I
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    chip8->i += chip8->v[reg];
                    break;
                }

                case 0x0029: {
                    // set I to the sprite of X
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    chip8->i = FONT_START + 5 * chip8->v[reg];
                    break;
                }

                case 0x0033: {
                    // store the binary representation of X in I, I + 1, I + 2
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    unsigned char num = chip8->v[reg];
                    unsigned char hundreds = num / 100;
                    unsigned char tens = num / 10 - hundreds * 10;
                    unsigned char ones = num - hundreds * 100 - tens * 10;
                    chip8->memory[chip8->i] = hundreds;
                    chip8->memory[chip8->i + 1] = tens;
                    chip8->memory[chip8->i + 2] = ones;
                    break;
                }

                case 0x0055: {
                    // store V0 to VX in memory starting at I
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    for(int i = 0; i <= reg; i++) {
                        if(sFlag)
                            chip8->memory[chip8->i + i] = chip8->v[i];
                        else
                            chip8->memory[chip8->i++] = chip8->v[i];
                    }
                    break;
                }

                case 0x0065: {
                    // read from memory starting at I into V0 to VX
                    unsigned char reg = (instruction & 0x0F00) >> 8;
                    for(int i = 0; i <= reg; i++) {
                        if(sFlag)
                            chip8->v[i] = chip8->memory[chip8->i + i];
                        else
                            chip8->v[i] = chip8->memory[chip8->i++];
                    }
                    break;
                }

                default: {
                    return instruction;
                }
            }
            break;
        }

        default: {
            return instruction;
        }
    }

    return -1;
}

void chip8AssignSDLKey(Chip8* chip8, SDL_Keycode key, unsigned char state) {
    switch(key) {
        case SDLK_1: {
            chip8->keyboard[0x1] = state;
            chip8->keyFlag = state ? 0x1 : chip8->keyFlag;
            break;
        }

        case SDLK_2: {
            chip8->keyboard[0x2] = state;
            chip8->keyFlag = state ? 0x2 : chip8->keyFlag;
            break;
        }

        case SDLK_3: {
            chip8->keyboard[0x3] = state;
            chip8->keyFlag = state ? 0x3 : chip8->keyFlag;
            break;
        }

        case SDLK_4: {
            chip8->keyboard[0xC] = state;
            chip8->keyFlag = state ? 0xC : chip8->keyFlag;
            break;
        }

        case SDLK_q: {
            chip8->keyboard[0x4] = state;
            chip8->keyFlag = state ? 0x4 : chip8->keyFlag;
            break;
        }

        case SDLK_w: {
            chip8->keyboard[0x5] = state;
            chip8->keyFlag = state ? 0x5 : chip8->keyFlag;
            break;
        }

        case SDLK_e: {
            chip8->keyboard[0x6] = state;
            chip8->keyFlag = state ? 0x6 : chip8->keyFlag;
            break;
        }

        case SDLK_r: {
            chip8->keyboard[0xD] = state;
            chip8->keyFlag = state ? 0xD : chip8->keyFlag;
            break;
        }

        case SDLK_a: {
            chip8->keyboard[0x7] = state;
            chip8->keyFlag = state ? 0x7 : chip8->keyFlag;
            break;
        }

        case SDLK_s: {
            chip8->keyboard[0x8] = state;
            chip8->keyFlag = state ? 0x8 : chip8->keyFlag;
            break;
        }

        case SDLK_d: {
            chip8->keyboard[0x9] = state;
            chip8->keyFlag = state ? 0x9 : chip8->keyFlag;
            break;
        }

        case SDLK_f: {
            chip8->keyboard[0xE] = state;
            chip8->keyFlag = state ? 0xE : chip8->keyFlag;
            break;
        }

        case SDLK_z: {
            chip8->keyboard[0xA] = state;
            chip8->keyFlag = state ? 0xA : chip8->keyFlag;
            break;
        }

        case SDLK_x: {
            chip8->keyboard[0x0] = state;
            chip8->keyFlag = state ? 0x0 : chip8->keyFlag;
            break;
        }

        case SDLK_c: {
            chip8->keyboard[0xB] = state;
            chip8->keyFlag = state ? 0xB : chip8->keyFlag;
            break;
        }

        case SDLK_v: {
            chip8->keyboard[0xF] = state;
            chip8->keyFlag = state ? 0xF : chip8->keyFlag;
            break;
        }
    }
}
