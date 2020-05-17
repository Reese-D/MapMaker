CC=gcc
C=./src/code/
H=./src/headers/
GL=./src/shaders/
BIN=./bin/
IFLAGS=-lglfw3 -lGLEW -lGL -lpthread -lX11 -ldl -lm
DIRS=-Isrc/headers/

make link: main $(BIN)HelloWorld.o
	ln -s $(BIN)HelloWorld.o HelloWorld

main: $(H)HelloWorld.h $(C)HelloWorld.c shaderLoader matrixOperations bin
	$(CC) $(DIRS) $(C)HelloWorld.c $(IFLAGS) -o $(BIN)HelloWorld.o $(BIN)ShaderLoader.o $(BIN)ReadFile.o $(BIN)MatrixOperations.o

shaderLoader: $(H)ShaderLoader.h $(C)ShaderLoader.c readFile bin
	$(CC) $(DIRS) -c $(C)ShaderLoader.c -lGLEW -o $(BIN)ShaderLoader.o 

readFile: $(H)ReadFile.h $(C)ReadFile.c bin
	$(CC) $(DIRS) -c $(C)ReadFile.c -o $(BIN)ReadFile.o

matrixOperations: $(H)MatrixOperations.h $(C)MatrixOperations.c bin
	$(CC) $(DIRS) -c $(C)MatrixOperations.c -lGLEW -o $(BIN)MatrixOperations.o

bin:
	mkdir bin

.PHONY: clean

clean:
	rm $(BIN)HelloWorld.o $(BIN)ShaderLoader.o HelloWorld


