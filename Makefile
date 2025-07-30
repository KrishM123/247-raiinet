CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -MMD -MP -I/opt/X11/include
LDFLAGS = -L/opt/X11/lib -lX11

BUILD_DIR = build
EXEC = raiinet

SRCS := $(shell find . -name "*.cc" -not -path "./$(BUILD_DIR)/*")

OBJS := $(SRCS:./%.cc=$(BUILD_DIR)/%.o)

DEPS := $(OBJS:.o=.d)

VPATH := $(sort $(dir $(SRCS)))

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: %.cc
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXEC)

-include $(DEPS) 