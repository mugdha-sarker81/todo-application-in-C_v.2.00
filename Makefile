CC = gcc
# gnu11 (not c11) because export.c uses raw string literals R"(...)"
CFLAGS ?= -std=gnu11 -Wall -Wextra

ifeq ($(OS),Windows_NT)
TARGET = todo.exe
RUN = $(TARGET)
else
TARGET = todo
RUN = ./$(TARGET)
endif

SOURCES = main.c tasks.c habits.c calendar.c export.c team.c team_auth.c
OBJECTS = $(SOURCES:.c=.o)
HEADERS = tasks.h habits.h calendar.h export.h team.h team_auth.h

.PHONY: all run clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# Rebuild an object file when its .c file or any header changes
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	$(RUN) $(ARGS)

clean:
	$(RM) todo todo.exe
