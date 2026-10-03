CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -Icabecalhos # para o GCC encontrar o .h
LDLIBS = -lm

TARGET = a.out
SRC = $(wildcard fontes/*.c) # pega automaticamente os .c de fontes

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDLIBS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
