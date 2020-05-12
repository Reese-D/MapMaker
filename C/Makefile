CC=gcc
C=./src/code/
H=./src/headers/
GL=./src/shaders/
BIN=./bin/
IFLAGS=-lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl -lm
main: $(H)HelloWorld.h $(C)HelloWorld.c shaderLoader bin
	$(CC) $(C)HelloWorld.c $(IFLAGS) -o $(BIN)HelloWorld.o

shaderLoader: $(H)ShaderLoader.h $(C)ShaderLoader.c bin
	$(CC) -c $(C)ShaderLoader.c -lGLEW -o $(BIN)ShaderLoader.o

bin:
	mkdir bin

.PHONY: clean

clean:
	rm $(BIN)HelloWorld.o $(BIN)ShaderLoader.o


