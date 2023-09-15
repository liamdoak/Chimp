#define SDL_MAIN_HANDLED

#include <math.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include "chip8.h"
#include "display.h"
#include "main.h"

#define PI2 6.28318530718

int main(int argc, char** argv) {
    // seed rand
    srand(time(0));

    // start program clock
    struct timespec clockStart;
    timespec_get(&clockStart, TIME_UTC);

    // check if a ROM file is given
    if(argc < 2) {
        printf("ERROR: no ROM file specified\n");

        return -1;
    }

    // flags
    // debug mode flag
    unsigned char dbgFlag = 0;

    if(argc == 3 && argv[2][0] == 'd') {
        dbgFlag = 1;
    }

    // switch to S-CHIP/CHIP-48 instruction versions
    unsigned char sFlag = 0;

    if(argc == 3 && argv[2][1] == 's') {
        sFlag = 1;
    }

    // SDL setup
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("ERROR: could not initialize SDL\n");
        printf("SDL ERROR: %s\n", SDL_GetError());

        return -1;
    }

    // SDL window creation
    SDL_Window* window = SDL_CreateWindow("Chimp", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED, DISPLAY_WIDTH * WINDOW_SCALE, DISPLAY_HEIGHT *
        WINDOW_SCALE, SDL_WINDOW_SHOWN);

    if(!window) {
        printf("ERROR: could not create an SDL window\n");
        printf("SDL ERROR: %s\n", SDL_GetError());

        return -1;
    }

    // SDL audio setup
    AudioData audioData;
    audioData.frequency = 261.6;
    audioData.time = 0;

    SDL_AudioSpec spec, aspec;
    SDL_zero(spec);
    spec.freq = 44100;
    spec.format = AUDIO_S16SYS;
    spec.channels = 1;
    spec.samples = 4096;
    spec.callback = audioCallback;
    spec.userdata = &audioData;

    SDL_AudioDeviceID audioID;
    if((audioID = SDL_OpenAudioDevice(0, 0, &spec, &aspec, 0)) <= 0) {
        printf("ERROR: could not open audio\n");
        printf("SDL ERROR: %s\n", SDL_GetError());

        return -1;
    }

    SDL_PauseAudioDevice(audioID, 1);

    // Chip8 setup
    Chip8* chip8 = chip8Create();

    for(int i = 0; i < DISPLAY_WIDTH * DISPLAY_HEIGHT / 8; i++) {
        chip8->display[i] = 0;
    }

    // SDL window screen setup
    Buffer* buffer = bufferCreate(DISPLAY_WIDTH, DISPLAY_HEIGHT, WINDOW_SCALE);
    bufferUpdate(buffer, chip8);
    SDL_Surface* sdlBuffer = SDL_CreateRGBSurfaceFrom((void*) buffer->data,
        buffer->width, buffer->height, 32, 4 * buffer->width, 0x000000FF,
        0x0000FF00, 0x00FF0000, 0x00000000);
    SDL_Surface* screen = SDL_GetWindowSurface(window);

    // Load ROM
    if(!chip8LoadROM(chip8, argv[1])) {
        printf("ERROR: could not find file %s\n", argv[1]);

        return -1;
    }

    // render loop
    unsigned char done = 0;
    unsigned char soundStarted = 0;
    unsigned char dbgRunInstruction = 0;
    double secondsPerChip8Tick = 1.0 / CHIP8_CLOCK_SPEED * 1000000000LL;
    double secondsPerUpdateTick = 1.0 / 60.0 * 1000000000LL;

    struct timespec clock;
    timespec_get(&clock, TIME_UTC);

    unsigned long long previousChip8Tick = (clock.tv_sec - clockStart.tv_sec) *
        1000000000LL + (clock.tv_nsec - clockStart.tv_nsec);
    unsigned long long previousUpdateTick = previousChip8Tick;

    while(!done) {
        SDL_Event e;

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT: {
                    done = 1;
                    break;
                }

                case SDL_KEYDOWN: {
                    chip8AssignSDLKey(chip8, e.key.keysym.sym, 1);

                    if(dbgFlag && e.key.keysym.sym == SDLK_p)
                        dbgRunInstruction = 1;
                    if(dbgFlag && e.key.keysym.sym == SDLK_o) {
                        printf("--  i | %.4X\n", chip8->i);
                        for(int i = 0; i < 16; i++) {
                            printf("-- %.2X | %.2X %d\n", i, chip8->v[i],
                                chip8->v[i]);
                        }
                    }

                    break;
                }

                case SDL_KEYUP: {
                    chip8AssignSDLKey(chip8, e.key.keysym.sym, 0);
                    break;
                }
            }
        }

        timespec_get(&clock, TIME_UTC);

        unsigned long long currentTick = (clock.tv_sec - clockStart.tv_sec) *
            1000000000LL + (clock.tv_nsec - clockStart.tv_nsec);

        // runs at CHIP8_CLOCK_SPEED Hz
        if((!dbgFlag && currentTick - previousChip8Tick >= secondsPerChip8Tick)
            || (dbgFlag && dbgRunInstruction)) {
            dbgRunInstruction = 0;
            previousChip8Tick = currentTick;

            int invalidInstruction = 0;
            if((invalidInstruction = chip8FDE(chip8, dbgFlag, sFlag)) != -1)
                printf("ERROR: invalid instruction %.4X\n",
                    invalidInstruction);

            chip8->keyFlag = -1;
        }

        // runs at 60 Hz
        if(currentTick - previousUpdateTick >= secondsPerUpdateTick) {
            previousUpdateTick = currentTick;

            if(chip8->delayTimer > 0)
                chip8->delayTimer--;

            if(chip8->soundTimer > 0) {
                chip8->soundTimer--;

                if(!soundStarted) {
                    soundStarted = 1;
                    SDL_PauseAudioDevice(audioID, 0);
                }
            }

            else {
                soundStarted = 0;
                SDL_PauseAudioDevice(audioID, 1);
            }

            bufferUpdate(buffer, chip8);
            SDL_BlitSurface(sdlBuffer, 0, screen, 0);
            SDL_UpdateWindowSurface(window);
        }
    }

    // Chip8 cleanup
    chip8Destroy(chip8);

    // buffer cleanup
    bufferDestroy(buffer);

    // SDL cleanup
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Buffer* bufferCreate(unsigned int width, unsigned int height,
    unsigned int scale) {
    Buffer* buffer = malloc(sizeof(Buffer));
    buffer->data = malloc(sizeof(unsigned int) * width * height * scale *
        scale);
    buffer->width = width * scale;
    buffer->height = height * scale;
    buffer->scale = scale;

    return buffer;
}

void bufferDestroy(Buffer* buffer) {
    free(buffer->data);
    free(buffer);
}

void bufferUpdate(Buffer* buffer, Chip8* chip8) {
    for(int y = 0; y < buffer->height; y++) {
        for(int x = 0; x < buffer->width; x++) {
            unsigned char bit = displayGetBit(chip8->display, x / buffer->scale,
                y / buffer->scale);
            if(bit == 0)
                buffer->data[y * buffer->width + x] = 0x00000000;
            else
                buffer->data[y * buffer->width + x] = 0xFFFFFFFF;
        }
    }
}

void audioCallback(void* userData, Uint8* stream, int len) {
    AudioData* audioData = userData;
    short* buffer = (short*) stream;
    len /= sizeof(short);

    for(int i = 0; i < len; i++) {
        buffer[i] = 32000 * sinf(audioData->time);
        audioData->time += audioData->frequency * PI2 / 44100.0;

        if(audioData->time >= PI2)
            audioData->time -= PI2;
    }
}
