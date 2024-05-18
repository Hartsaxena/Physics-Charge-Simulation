CC=g++
SDLINCLUDE=-lmingw32 -lSDL2main -lSDL2
SRC=$(wildcard *.cpp)


all: main.exe
	main.exe | tee log.txt

main.exe: $(SRC:.cpp=.o)
	$(CC) -Isdl\include -Lsdl\lib -o $@ $(SRC:.o=.o) -Iinclude $(SDLINCLUDE) -Wall

main.o: main.cpp
	$(CC) -Isdl\include -Lsdl\lib -c $^ -Iinclude $(SDLINCLUDE) -Wall

charges.o : charges.cpp
	$(CC) -Isdl\include -Lsdl\lib -c $^ -Iinclude $(SDLINCLUDE) -Wall

front.o : front.cpp
	$(CC) -Isdl\include -Lsdl\lib -c $^ -Iinclude $(SDLINCLUDE) -Wall

clean:
	del *.o
	del *.exe
	del log.txt