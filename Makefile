CXX = g++
CXXFLAGS = -std=c++17 -Wall -g
LDFLAGS =
BUILD_DIR = build

# Find all .cc files in the project directory and its subdirectories
SRCS = $(shell find . -name "*.cc")
# Create object file names in the build directory, e.g. build/main.o
OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(SRCS:.cc=.o)))
EXEC = $(BUILD_DIR)/raiinet

# VPATH tells make where to find source files
VPATH = $(sort $(dir $(SRCS)))

.PHONY: all clean

all: $(EXEC)

# Rule to link the executable.
# The | $(BUILD_DIR) is an order-only prerequisite.
$(EXEC): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to create the build directory.
$(BUILD_DIR):
	mkdir -p $@

# Rule to compile a .cc file into a .o file in the build directory.
$(BUILD_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) 