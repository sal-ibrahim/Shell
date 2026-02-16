# Makefile for modular Unix shell
#
# This Makefile builds the shell from modular source files organized
# by responsibility.

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_POSIX_C_SOURCE=200809L -I./include -g
LDFLAGS = -lreadline

# Target executable
TARGET = myshell

# Source directories
SRC_DIRS = src \
           src/builtins \
           src/executor \
           src/parser \
           src/path \
           src/completion

# Find all .c files in source directories
SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Generate object file names
OBJECTS = $(SOURCES:.c=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET) $(OBJECTS:.o=.d)
	@echo "Clean complete"


# Rebuild everything
rebuild: clean all

# Show build information
info:
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"
	@echo "Compiler: $(CC) $(CFLAGS)"
	@echo "Linker flags: $(LDFLAGS)"

# Install (optional - copies to /usr/local/bin)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete"

# Run the shell
run: $(TARGET)
	./$(TARGET)

# Phony targets (not actual files)
.PHONY: all clean rebuild info install run

# Dependency tracking (automatic header dependencies)
# This tells make to rebuild .o files if headers change
DEPFLAGS = -MMD -MP
CFLAGS += $(DEPFLAGS)

# Include generated dependency files
-include $(OBJECTS:.o=.d)
