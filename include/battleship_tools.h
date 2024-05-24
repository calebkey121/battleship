// This file defines auxiliary utilities for battleship

#ifndef BATTLESHIP_TOOLS_H
#define BATTLESHIP_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <math.h>

#include "file_stuff.h"

int end_game_early( FILE *fp, const char *msg, ...);
void get_battleship_env( int *start_x, int *start_y, int *length, bool *horizontal );
bool is_in_range( int x, int lower, int upper);
int rand_number_inclusive( int lower, int upper );

#endif
