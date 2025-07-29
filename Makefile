CXX = g++
# -MMD -MP flags generate dependency files for headers
CXXFLAGS = -std=c++17 -Wall -g -MMD -MP
# Linker flags. For X11 graphics, you'll likely need -lX11
LDFLAGS = -lX11

BUILD_DIR = build
EXEC = $(BUILD_DIR)/raiinet

# Find all .cc files, excluding the build directory
SRCS := $(shell find . -name "*.cc" -not -path "./$(BUILD_DIR)/*")

# Recreate the directory structure in the build directory for object files
# e.g., ./ability/ability.cc -> $(BUILD_DIR)/ability/ability.o
OBJS := $(SRCS:./%.cc=$(BUILD_DIR)/%.o)

# Dependency files for each object
DEPS := $(OBJS:.o=.d)

# Tell make where to find source files, like the original VPATH
VPATH := $(sort $(dir $(SRCS)))

.PHONY: all clean

all: $(EXEC)

# Rule to link the executable.
# It also creates the build directory if it doesn't exist.
$(EXEC): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

# Rule to compile a .cc file into a .o file in the build directory.
# The target directory is created before compiling.
$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

# Include the dependency files. The - tells make to ignore errors
# if the file doesn't exist (e.g., on the first build).
-include $(DEPS) 