WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer
OPTIMIZE = -O0
CFLAGS =
CC = gcc
BIN_DIR = ./obj
SRC_DIR = ./src
HEADERS_DIR = ./header
INCLUDES = -I $(HEADERS_DIR)

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
HEADER_FILES=$(wildcard $(HEADER_DIR)/*.h)
OBJECTS=$(SRC_FILES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

EXECUTABLE = lab_1

TESTING_DIR = ./testing


all: $(EXECUTABLE) test
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -o $(EXECUTABLE) $(OBJECTS)

$(BIN_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -c -o $@ $(SRC_DIR)/main.c
	
$(BIN_DIR)/ObjectLinkedList.o: $(SRC_DIR)/ObjectLinkedList.c $(HEADERS_DIR)/ObjectLinkedList.h
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -c -o $@ $(SRC_DIR)/ObjectLinkedList.c
	
$(BIN_DIR)/Object.o: $(SRC_DIR)/Object.c $(HEADERS_DIR)/Object.h
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -c -o $@ $(SRC_DIR)/Object.c
	
$(BIN_DIR)/ObjectLinkedStack.o: $(SRC_DIR)/ObjectLinkedStack.c $(HEADERS_DIR)/ObjectLinkedStack.h
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -c -o $@ $(SRC_DIR)/ObjectLinkedStack.c

$(BIN_DIR)/ObjectArrayStack.o: $(SRC_DIR)/ObjectArrayStack.c $(HEADERS_DIR)/ObjectArrayStack.h
	$(CC) $(INCLUDES) $(DEBUG) $(CFLAGS) $(WARNINGS) $(OPTIMIZE) -c -o $@ $(SRC_DIR)/ObjectArrayStack.c	

clean:
	rm -vfr lab_1 $(BIN_DIR)/*.o
	rm $(EXECUTABLE)
	make -C $(TESTING_DIR) clean
	
test: $(OBJECTS)
	make -j8 -C $(TESTING_DIR) all
	make -C $(TESTING_DIR) run

run:
	./lab_1
