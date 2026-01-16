# Compiler and flags
CC      = gcc
CFLAGS  = -Wall -g

# Target executable
TARGET  = mysh

# Source and object files
SRC     = src/main.c src/parser.c src/builtins.c src/path.c src/process.c
OBJ     = $(SRC:.c=.o)

# Default target
all: $(TARGET)

# Link step
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Compile step
src/%.o: src/%.c src/shell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all

.PHONY: all clean fclean re
