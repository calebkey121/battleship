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
#include "board.h"
#include "boat.h"

int random_game_loop( Board *game_board, int num_hits, int game_num, FILE *fp );
int manual_game_loop( Board *game_board, int game_num, FILE *fp );
int random_run( Board *game_board, Boat *boats, int num_boats, int total_hits_required, FILE *fp);
bool set_board( Board *game_board, Boat *boats, int num_boats, FILE *fp );

#endif
