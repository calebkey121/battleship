// This file defines resources used between all files

#ifndef COMMON_H
#define COMMON_H

#define RANDOM_ENABLED // dont define for user controlled game

#define BUFFER_SIZE 10
#define BOARD_LENGTH 10
#define EMPTY_CHAR '~'
#define HIT_CHAR 'X'
#define MISS_CHAR 'O'
#define HORIZONTAL_CHAR '_'
#define VERTICAL_CHAR '|'
#define NUM_RUNS 20000 // if random, number of times game is played
#define LAST_RUN_FILENAME "../last_run.txt"
#define SUMMARY_FILENAME "../summary.txt"

struct boat {
    int start_coords[2];
    int length;
    bool horizontal; // might seem weird but wanted to display different chars for horizontal vs vertical
};

#endif
