CXX = g++
CXXFLAGS = -std=c++17 -Wall -I h -c -ggdb -fno-omit-frame-pointer
LXXFLAGS = -std=c++17 -I h -pthread
GTEST = /usr/local/lib/libgtest.a
TARGET = testing
LIBS = `pkg-config --cflags --libs gtest` -I ../header -I ./header

BIN_DIR=obj
SRC_DIR=tests

TEST_SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp)
TEST_OBJECTS=$(TEST_SRC_FILES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

EXTERNAL_BIN_DIR=../obj
EXTERNAL_OBJECTS=$(filter-out $(EXTERNAL_BIN_DIR)/main.o, $(wildcard $(EXTERNAL_BIN_DIR)/*))

all: $(TARGET)

$(TARGET): $(TEST_OBJECTS) $(EXTERNAL_OBJECTS)
	$(CXX) $(LIBS) $(LXXFLAGS) -o $(TARGET) $(TEST_OBJECTS) $(EXTERNAL_OBJECTS)
	
$(TEST_OBJECTS): $(TEST_SRC_FILES)
	$(CXX) $(LIBS) $(CXXFLAGS) $(@:$(BIN_DIR)/%.o=$(SRC_DIR)/%.cpp) -o $@

clean:
	rm -fv $(TARGET) $(TEST_OBJECTS) 

run: $(TARGET)
	./$(TARGET)
