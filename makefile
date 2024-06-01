CC=g++
SDLINCLUDE=-lmingw32 -lSDL2main -lSDL2
SRC=$(wildcard *.cpp)


run: clean main.exe
	main.exe

log: clean main.exe
	@cmd /c "main.exe > log.txt 2>&1"

debug: clean debug_main.exe
	gdb -ex run debug_main.exe

debug_main.exe:
	$(CC) -Isdl\include -Lsdl\lib -o $@ $(SRC) -Iinclude $(SDLINCLUDE) -Wall -g -O0

main.exe:
	$(CC) -Isdl\include -Lsdl\lib -o $@ $(SRC) -Iinclude $(SDLINCLUDE) -Wall

clean:
	del *.exe
	del log.txt