all: bin\chimp.exe

bin\chimp.exe: obj\main.obj obj\chip8.obj obj\display.obj obj\memory.obj
	link /out:bin\chimp.exe lib\SDL2.lib obj\main.obj obj\chip8.obj \
	obj\display.obj obj\memory.obj /subsystem:console

obj\main.obj: src\main.c src\chip8.h src\display.h src\main.h
	cl /c /Fo:obj\main.obj /Iinclude src\main.c

obj\chip8.obj: src\chip8.c src\chip8.h src\display.h src\memory.h
	cl /c /Fo:obj\chip8.obj /Iinclude src\chip8.c

obj\display.obj: src\display.c src\display.h
	cl /c /Fo:obj\display.obj /Iinclude src\display.c

obj\memory.obj: src\memory.c src\memory.h
	cl /c /Fo:obj\memory.obj /Iinclude src\memory.c

clean:
	erase /Q bin\chimp.exe
	erase /Q obj\*
