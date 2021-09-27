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

EXECUTABLE = lab_2

all: $(EXECUTABLE) Makefile

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(PREPROCESSOR) $(C_FLAGS) $(ADDICTIONAL_LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -o $@ $(OBJECTS)
	
$(OBJECTS): $(SRC_FILES) $(HEADER_FILES)
	$(CC) $(PREPROCESSOR) $(C_FLAGS) $(ADDICTIONAL_LIBS) $(WARNINGS) $(DEBUG) $(OPTIMIZE) -c -o $@ $(@:$(BIN_DIR)/%.o=$(SRC_DIR)/%.c)
	
clean:
	rm -f $(EXECUTABLE)
	rm $(OBJECTS)

run:
	./$(EXECUTABLE)
