#ifndef BOARD_H
#define BOARD_H

#include <stdio.h>
#include <stdbool.h>
#include "common.h"
#include "battleship_tools.h"


typedef struct {
    char board[BOARD_HEIGHT][BOARD_WIDTH + 1]; // +1 for null terminator
} Board;

void print_board( Board *game_board, FILE *fp );
bool on_board(int x, int y);
bool fire( Board *game_board, Coords coords, FILE *fp );


#endif
