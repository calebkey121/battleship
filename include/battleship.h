// Targeting 6.5, 6.7, 6.8, 6.10, 6.17, 6.19, 6.20
// This file defines functions for the management of the game directly

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>

#include "common.h"
#include "battleship_tools.h"
#include "file_stuff.h"


bool fire( char board[BOARD_LENGTH][BOARD_LENGTH + 1], int target_coords[2], FILE *fp );
int game_loop( char board[BOARD_LENGTH][BOARD_LENGTH + 1], int num_hits, int game_num, FILE *fp );
int run_loop( char board[BOARD_LENGTH][BOARD_LENGTH + 1], struct boat b, FILE *fp);
void print_file_information( const char *filename );


#endif
