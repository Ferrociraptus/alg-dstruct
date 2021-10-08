WARNINGS = -Wall
DEBUG = -ggdb -fno-omit-frame-pointer -Walloc-zero -Walloca -Wformat-overflow -Wformat-truncation -Wnonnull
OPTIMIZE = -O0
CC = gcc
ADDICTIONAL_LIBS =
C_FLAGS = -I$(HEADER_DIR) -I../lab2

SRC_DIR=src
BIN_DIR=bin
HEADER_DIR=head

SRC_FILES=$(wildcard $(SRC_DIR)/*.c)
HEADER_FILES=$(wildcard $(HEADER_DIR)/*.h)
OBJECTS=$(SRC_FILES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)

TESTING_DIR = ./testing

EXECUTABLE = lab_3

all: $(EXECUTABLE) test

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(PREPROCESSOR) $(C_FLAGS) $(ADDICTIONAL_LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $@ $(OBJECTS)
	
$(OBJECTS): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(PREPROCESSOR) $(C_FLAGS) $(ADDICTIONAL_LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -c -o $@ $(@:$(BIN_DIR)/%.o=$(SRC_DIR)/%.c)
	
clean:
	rm -vfr lab_1 $(BIN_DIR)/*.o
	rm $(EXECUTABLE)
	make -C $(TESTING_DIR) clean
	
test: $(OBJECTS)
	make -j8 -C $(TESTING_DIR) all
	make -C $(TESTING_DIR) run

run:
	./$(EXECUTABLE)
