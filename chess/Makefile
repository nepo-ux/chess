CC=gcc

CFLAGS=-c
LDFLAGS=
#for server version add server.c
#and go to main.c uncomment server() and comment everything else 
SOURCES=main.c board.c data.c hashkeys.c check.c init.c mate.c validmove.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=chess

BUILD_DIR=build
BIN_DIR=bin

BUILD_OBJECTS=$(addprefix $(BUILD_DIR)/, $(OBJECTS))

all: $(BIN_DIR)/$(EXECUTABLE)

$(BIN_DIR)/$(EXECUTABLE): $(BUILD_OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

-include $(BUILD_OBJECTS:.o=.d)

clean:
	rm -rf $(BUILD_DIR)

build: $(BIN_DIR)/$(EXECUTABLE)

.PHONY: all clean build
