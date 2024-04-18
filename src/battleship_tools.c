#include "battleship_tools.h"

// print to both file and console (stdout, stderr) with a format string like printf
void output( FILE *file, FILE *console, const char *formatstring, ... ) {
    va_list args;

    va_start(args, formatstring);
    vfprintf(console, formatstring, args); // need this to print like printf does
    va_end(args);

    va_start(args, formatstring);
    vfprintf(file, formatstring, args); // need this to print like printf does
    va_end(args); 
}

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

// file stuff

// pretty simple but more readable than using access everywhere
bool file_exists( const char *filename ) {
    // https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
    if (access(filename, F_OK) == 0) {
        // file exists
        return true;
    } else {
        // file doesn't exist
        return false;
    }
}

FILE *get_summary_file() {
    bool s_file_exists = file_exists(SUMMARY_FILENAME);
    FILE *fp = fopen(SUMMARY_FILENAME, "r+");
    if ( fp == NULL ) {
            return NULL;
    }
    if ( !s_file_exists ) { // write basic information
        fprintf(fp, ">>>--- Start Summary ---<<<\n\n"\
                            "Total number of shots: 0\n"\
                            "Highest shots in single game: 0\n"\
                            "Fewest shots in single game: -1\n"\
                            "Average number of shots: 0\n\n"\
                            ">>>---  End Summary  ---<<<\n");
    }
    return fp;
}

int write_summary( int total_shots, int max_num_shots, int min_num_shots, float average_num_shots) {
    // return 0 for success, non zero return for some error
    FILE *fp = get_summary_file();
    if ( fp == NULL ) {
        return 1;
    }
    return 0;
}
