// This file defines auxiliary utilities for battleship

#ifndef BATTLESHIP_TOOLS_H
#define BATTLESHIP_TOOLS_H

#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>

#include "common.h"

void output( FILE *file, FILE *console, const char *formatstring, ... );
bool set_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], struct boat b, FILE *fp );
void print_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], FILE *fp );
int end_game_early( FILE *fp, const char *msg, ...);

// file stuff
bool file_exists( const char *filename);
FILE *get_summary_file();
int write_summary(int total_shots, int max_num_shots, int min_num_shots, float average_num_shots);

#endif
