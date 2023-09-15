# Chimp
A simple CHIP-8 emulator.
This program fully emulates a CHIP-8 system according to the specification at https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Technical-Reference.

CHIP-8 is an interpreted language that mimics an 8-bit computer. This program is a chip-8 emulator that fetches 2 byte intructions from a file, decodes the instruction, then executes the instruction, mimicing a
Fetch-Decode-Execute cycle. This program also mimics the hardware like registers, memory, the display, audio, and the keyboard through data structures to emulate CHIP-8's interactions with hardware.

# Compilation
This program requires SDL to compile, and uses the Visual Studio compiler cl. Create a lib directory to add SDL to and create an include directory to add SDL's headers to, then compile using make.

# Using
To run the CHIP-8 emulator on a ROM file, run the command:\
`chimp <rom-file>`\
To enter debug mode, run the command:\
`chimp <rom-file> d-`\

To enter S-CHIP/CHIP-48 mode, run the command:\
`chimp <rom-file> -s`\

To enter both debug and S-CHIP mode, run the command:\
`chimp <rom-file> ds`\
