CC = gcc
CFLAGS = -std=c17 -pedantic -Wall
LDFLAGS = -lm -lMLV
SRC_DIR = src
OBJ_DIR = bin
INC_DIR = include
EXE = tower_defense

OBJ = $(OBJ_DIR)/grid.o $(OBJ_DIR)/main.o $(OBJ_DIR)/graph.o $(OBJ_DIR)/gem.o $(OBJ_DIR)/monster.o $(OBJ_DIR)/player.o $(OBJ_DIR)/game.o

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) -c $< -o $@ -g $(CFLAGS) -I $(INC_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) -c $< -o $@ -g $(CFLAGS) -I $(INC_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(EXE)

$(shell mkdir -p $(OBJ_DIR))