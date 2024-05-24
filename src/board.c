# include "board.h"


/**
 *  Output board state to stdout and optionally a file
 * 
 *  Description:
 *      Write each line of the board to stdout/file
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      void
 * 
 *  TODO:
 *      some checking to make sure the board we're told to output is as expected
 */
void print_board( Board *game_board, FILE *fp ) {
    // TODO: some checking to make sure board is what we're expecting
    // print y axis label
    int height_digits = floor(log10(BOARD_HEIGHT)) + 1; // number of digits BOARD_HEIGHT has
    printf("%*sy\n%*s^\n", height_digits, "", height_digits, "");
    for (int i = 0; i < BOARD_HEIGHT; i++) {
        int row = BOARD_HEIGHT - i;
        printf("%*d|%s\n", height_digits, row, game_board->board[i]);
    }

    // print the bottom row of the board
    // print bottom separator
    printf("%*s", height_digits, "");
    for (int i = 0; i <= BOARD_WIDTH; i++) {
        printf("%c", '-');
    }
    printf("> x\n");

    // print bottom row numbers
    int width_digits = floor(log10(BOARD_WIDTH)) + 1; // number of digits BOARD_WIDTH has
    for (int i = 0; i < height_digits; i++) {
        printf("%*s", height_digits, "");
        for (int j = 0; j <= BOARD_WIDTH; j++) {
            int j_digits = floor(log10(j)); // number of digits j has
            if ( i > j_digits ) { // if we're past the number of digits j has, print a space
                printf(" ");
            } else { // else print the digit in the i-th place
                printf("%d", (int)(floor(j / floor(pow(10, j_digits - i)))) % 10);
            }
        }
        printf("\n");
    }
}

bool on_board(int x, int y) {
    return x >= 0 && x < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT;
}


/**
 *  Ready, set, fire! Shoot at target coords and report if its a miss or hit!
 * 
 *  Description:
 *      Make sure target coords are valid, then check the board to see if they're a hit or miss. Output if we're not doing a random run
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      target_coords - where are we aiming? [x, y], 1 indexed
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      bool - true if shot is a hit, false if its a miss
 */
bool fire( Board *game_board, Coords target_coords, FILE *fp ) {
    // target_coords are 1 indexed, so we need to convert them to 0 indexed
    target_coords.x -= 1;
    target_coords.y -= 1;

    // to index into board, we need to flip y, since the user sees the board as [1, 1] in the bottom left corner
    target_coords.y = BOARD_HEIGHT - target_coords.y - 1;
    char *targeted = &game_board->board[target_coords.y][target_coords.x];
    if ( target_coords.x < 0 || target_coords.x > BOARD_WIDTH - 1 || target_coords.y < 0 || target_coords.y > BOARD_HEIGHT - 1 ) {
        output(fp, stdout, "target coordinates are outside of range :(\n");
        return false;
    }
    if ( *targeted == HORIZONTAL_CHAR || *targeted == VERTICAL_CHAR || *targeted == VERTICAL_UP_CHAR || *targeted == VERTICAL_DOWN_CHAR || *targeted == HORIZONTAL_LEFT_CHAR || *targeted == HORIZONTAL_RIGHT_CHAR) {
        game_board->board[target_coords.y][target_coords.x] = HIT_CHAR; // better make sure your input is clean
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "hit!\n");
        #endif
        return true;
    } else if ( game_board->board[target_coords.y][target_coords.x] == HIT_CHAR ) { // or miss char too, right?
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "You already hit there previously.. are you even paying attention!?!\n");
        #endif
        return false;
    } else {
        game_board->board[target_coords.y][target_coords.x] = MISS_CHAR; // better make sure your input is clean
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "miss...\n");
        #endif
        return false;
    }
}
