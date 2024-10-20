CC := cc
CFLAGS := -Wall -Wextra -pedantic -std=c99 -O2 $(shell sdl2-config --cflags)
LFLAGS := -lm $(shell sdl2-config --static-libs)
BUILD_DIR := ./build
SRC_DIR := ./src
OBJ_NAME := nes-tetris
$(shell mkdir -p $(BUILD_DIR))

.PHONY: all
all: $(BUILD_DIR)/main.o $(BUILD_DIR)/matrix.o $(BUILD_DIR)/graphics.o $(BUILD_DIR)/bot.o $(SRC_DIR)/errorvalues.h
	$(CC) $(CFLAGS) -o $(BUILD_DIR)/$(OBJ_NAME) $^ $(LFLAGS)

$(BUILD_DIR)/main.o: $(SRC_DIR)/main.c $(BUILD_DIR)/matrix.o $(BUILD_DIR)/graphics.o $(BUILD_DIR)/bot.o
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/graphics.o: $(SRC_DIR)/graphics.c $(SRC_DIR)/graphics.h $(BUILD_DIR)/matrix.o
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/bot.o: $(SRC_DIR)/bot.c $(SRC_DIR)/bot.h $(BUILD_DIR)/matrix.o $(SRC_DIR)/errorvalues.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/matrix.o: $(SRC_DIR)/matrix.c $(SRC_DIR)/matrix.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)