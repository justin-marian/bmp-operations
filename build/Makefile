# Compiler and compiler flags
CC = gcc
CFLAGS = -Wall -Werror

# Executable names
INTERACTIVE_EXEC = interactive

# Paths
SRC_PATH = ../src

# Source files
INTERACTIVE_SRC = $(SRC_PATH)/interactive.c $(SRC_PATH)/imageprocessing.c $(SRC_PATH)/bmp.c

# Object files
INTERACTIVE_OBJ = $(INTERACTIVE_SRC:$(SRC_PATH)/%.c=%.o)

# Phony targets
.PHONY: all clean run-main run-interactive

# Default target
all: $(INTERACTIVE_EXEC)

# Rule to run interactive program
run-interactive: $(INTERACTIVE_EXEC)
	./$(INTERACTIVE_EXEC)

# Tag to build interactive executable
$(INTERACTIVE_EXEC): $(INTERACTIVE_OBJ)
	$(CC) $(CFLAGS) $(INTERACTIVE_OBJ) -o $(INTERACTIVE_EXEC)

# Pattern matching compile source files
%.o: $(SRC_PATH)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to clean up object files and executables
clean:
	rm -rf result $(INTERACTIVE_OBJ) $(MAIN_EXEC) $(INTERACTIVE_EXEC) ../tests-out/*.bmp > /dev/null 2>&1
	@make -f Makefile.checker clean
