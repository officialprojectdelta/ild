TARGET_EXEC = ILD
CC = clang++
LINK = clang++ 
UNAME_S = $(shell uname -s)
HOME_DIR = ~/dev/ILD

SRC = $(wildcard src/*.cpp) $(wildcard src/**/*.cpp) $(wildcard src/**/**/*.cpp) $(wildcard src/**/**/**/*.cpp)
OBJ = $(SRC:.cpp=.o)
ASM = $(SRC:.cpp=.S)
BIN = bin

INC_DIR_SRC = -Isrc
INC_DIR_LIB =

DEBUGFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIB) -Wall -g
RELEASEFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIB) -O3
ASMFLAGS = $(INC_DIR_SRC) $(INC_DIR_LIBS) -Wall
LDFLAGS = $(LIBS) -lm

.PHONY: all libs clean

all: 
	$(MAKE) -j8 bld
	$(MAKE) link

dirs:
	mkdir -p ./$(BIN)

link: $(OBJ)
	@echo 'Building target executable:' $(TARGET_EXEC)
	@echo 'Invoking: MOLD Linker'
	$(LINK) -o $(BIN)/$(TARGET_EXEC) $^ $(LDFLAGS)
	@echo 'Finished building target executable:' $(TARGET_EXEC)
	@echo ' '

bld: 
	$(MAKE) clean
	$(MAKE) dirs
	$(MAKE) obj

obj: $(OBJ)

asm: cleanassembly $(ASM)

%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	$(CC) -std=c++2a -o $@ -c $< $(DEBUGFLAGS)
	@echo 'Finished building: $<'
	@echo ' '

%.S: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo 'Building ASM'
	$(CC) -std=c++2a -S -O -o $@ -c $< $(ASMFLAGS)
	@echo 'Finished building: $<'
	@echo ' '

build: dirs link

run:
	@echo 'Running executable: '$(TARGET_EXEC)
	./$(BIN)/$(TARGET_EXEC) Test.il;
	@echo 'Finished running executable: '$(TARGET_EXEC)
	@echo ' '

clean:
	clear
	rm -rf $(BIN) $(OBJ)

cleanassembly:
	rm -rf $(ASM)

