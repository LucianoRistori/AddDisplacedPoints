# ------------------------------------------------------------
# Makefile for AddDisplacedPoints (v3 with ROOT plotting)
# ------------------------------------------------------------

CXX      = clang++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -stdlib=libc++
INCLUDES = -I../common `root-config --cflags`
LDFLAGS  = `root-config --libs`

TARGET   = AddDisplacedPoints

SRCS     = AddDisplacedPoints.cpp \
           ../common/Points.cpp

OBJS     = $(SRCS:.cpp=.o)

# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------
all: $(TARGET)

# ------------------------------------------------------------
# Link
# ------------------------------------------------------------
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $(OBJS) $(LDFLAGS)

# ------------------------------------------------------------
# Compile
# ------------------------------------------------------------
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------
clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
