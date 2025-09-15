CXX := g++
CXXFLAGS := -std=c++17 -Werror -Wall -Wextra -I./src -I/usr/include
LDFLAGS := -lgtest -lgtest_main -lpthread

SRC_DIR := src
TEST_DIR := gtests
BUILD_DIR := build

TEST_SOURCES := $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TEST_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(TEST_SOURCES))

TEST_BIN := test_runner

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(TEST_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -rf $(BUILD_DIR) $(TEST_BIN)