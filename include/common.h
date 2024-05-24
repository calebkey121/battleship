// This file defines resources used between all files

#ifndef COMMON_H
#define COMMON_H

//#define RANDOM_ENABLED // dont define for user controlled game

#define EMPTY_CHAR '~'
#define HIT_CHAR 'X'
#define MISS_CHAR 'O'
#define HORIZONTAL_CHAR '-'
#define HORIZONTAL_LEFT_CHAR '<'
#define HORIZONTAL_RIGHT_CHAR '>'
#define VERTICAL_CHAR '|'
#define VERTICAL_UP_CHAR '^'
#define VERTICAL_DOWN_CHAR 'v'
#define MAX_ALLOWED_GAMES 3000 // if random, number of times game can be played in a single run
#define LAST_RUN_FILENAME "6_x/last_run.txt"
#define HISTORY_FILENAME "6_x/history.txt"

#ifndef BOARD_WIDTH
#define BOARD_WIDTH 10
#endif /* BOARD_WIDTH */
 
#ifndef BOARD_HEIGHT
#define BOARD_HEIGHT 5
#endif /* BOARD_HEIGHT */

#ifndef NUM_BOATS
#define NUM_BOATS 1
#endif /* NUM_BOATS */

#ifndef MAX_BOAT_LENGTH
#define MAX_BOAT_LENGTH 2
#endif /* MAX_BOAT_LENGTH */

#ifndef MIN_BOAT_LENGTH
#define MIN_BOAT_LENGTH 2
#endif /* MIN_BOAT_LENGTH */

typedef struct {
    int x;
    int y;
} Coords;

struct run_stats {
    int num_games;
    int *shots_per_game;
    int total_shots;
    int highest_shots;
    int fewest_shots;
    float average_num_shots;
};

#endif
