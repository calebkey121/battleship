// This file defines auxiliary utilities for battleship

#ifndef BATTLESHIP_TOOLS_H
#define BATTLESHIP_TOOLS_H

// make sure all of these are needed
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include "file_stuff.h"
#include "common.h"

bool set_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], struct boat b, FILE *fp );
void print_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], FILE *fp );
int end_game_early( FILE *fp, const char *msg, ...);

#endif
