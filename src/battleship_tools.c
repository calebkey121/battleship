#include "battleship_tools.h"


bool set_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], struct boat b, FILE *fp ) {
    // have already verified boat will fit on board
    for ( int i = 0; i < BOARD_LENGTH; i++ ) {
        for ( int j = 0; j < BOARD_LENGTH; j++ ) {
            board[i][j] = EMPTY_CHAR;
        } 
        board[i][BOARD_LENGTH] = '\0';
    }

    if ( b.start_coords[0] < 0 || b.start_coords[0] > BOARD_LENGTH - 1 || b.start_coords[1] < 0 || b.start_coords[1] > BOARD_LENGTH - 1 ) {
        output(fp, stdout, "starting boat coordinates are outside of range :(\n");
        return false;
    } 
    if ( b.horizontal && ((b.start_coords[1] + b.length) > (BOARD_LENGTH)) ) {
        output(fp, stdout, "boat does not fit on board :(\n");
        return false;
    } else if ( !b.horizontal && ((b.start_coords[0] + b.length) > (BOARD_LENGTH)) ) {
        output(fp, stdout, "boat does not fit on board :(\n");
        return false;
    } // else boat fits!
    
    char *curr = &board[b.start_coords[0]][b.start_coords[1]];
    for ( int i = 0; i < b.length; i++ ) {
        if ( b.horizontal ) {
            *curr = HORIZONTAL_CHAR;
            curr += 1;
        } else { // vertical - increase by length of board
            *curr = VERTICAL_CHAR;
            curr += BOARD_LENGTH + 1;
        }
    }
    return true;
}


void print_board( char board[BOARD_LENGTH][BOARD_LENGTH + 1], FILE *fp ) {
    // TODO: some checking to make sure board is what we're expecting
    for (int i = 0; i < BOARD_LENGTH; i++) {
        if ( fp == NULL ) {
            printf("%s\n", board[i]);
        } else {
            output(fp, stdout, "%s\n", board[i]);
        }
    }
}


int end_game_early( FILE *fp, const char *msg, ...) {
    va_list args;

    output(fp, stderr, msg, args);
    output(fp, stdout, "\n>>>--- End Game ---<<<\n");
    fclose(fp);
    return 1;
}
