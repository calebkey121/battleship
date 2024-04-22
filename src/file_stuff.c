#include "file_stuff.h"

/*
read/write may be done by other objectives.
Modify - Needs to preserve original file. Needs to be more than just append or overwrite from beginning. Needs to modify in a way a person would expect, ie don't stop at the original end if told to put in a long string.
Print file information to the console - stat type stuff, not contents.
Insert data into an existing file - Preserve original file. Preserve original contents. Insert into not beginning or end.

read/write will be done by other objectives.
Modify - Needs to preserve original file. Needs to be more than just append or overwrite from beginning. Needs to modify in a way a person would expect, ie don't stop at the original end if told to put in a long string.
Print file information to the console - stat type stuff, not contents.
Insert data into an existing file - Preserve original file. Preserve original contents. Insert into not beginning or end.
*/


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


FILE *get_history_file() {
    bool s_file_exists = file_exists(HISTORY_FILENAME);
    FILE *fp = fopen(HISTORY_FILENAME, "a+");
    if ( fp == NULL ) {
            return NULL;
    }
    if ( !s_file_exists ) { // write base information
        fprintf(fp, ">>>--- Start History ---<<<\n\n"\
                    "\n>>>---  End History  ---<<<\n"\
                    ">>>--- Start History Stats ---<<<\n\n"\
                    "Total number of shots: 0\n"\
                    "Highest shots in single game: 0\n"\
                    "Fewest shots in single game: -1\n"\
                    "Average number of shots: 0\n"\
                    "\n>>>---  End History Stats  ---<<<\n");
    }

    return fp;
}


int insert_latest_run( FILE *fp, struct run_stats stats ) {
    /*
    Move fp to insertion point, read rest of file into buffer, write new line, write rest of file
    */
    // assuming fp has been opened
    fseek(fp, 0, SEEK_SET);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    long pos;

    int skip = 2; // skip first two lines to get to insertion point
    for ( int i = 0; i < skip; i++ ) {
        if ( (getline(&line, &len, fp)) == -1 ) {
            return 1;
        }
    }

    // We need to read the rest of the file 
    // get file size
    struct stat file_stats;
    if ( stat(HISTORY_FILENAME, &file_stats) != 0 ) {
        perror("Failed to get file information\n");
        return 1;
    }

    long buffer_size = file_stats.st_size;
    char *buffer = malloc(buffer_size);
    if ( buffer == NULL ) {
        perror("Failed to allocate memory");
        fclose(fp);
        return 1;
    }
    return 0;
}


// https://codeforwin.org/c-programming/c-program-find-file-properties-using-stat-function
// TODO: print more stuff
int print_file_information( const char *filename ) {
    struct stat stats;
    if ( stat(filename, &stats) == 0 ) {
        printf("File size: %lld bytes\n", stats.st_size);
    } else {
        perror("Failed to get file information\n");
        return 1;
    }
    return 0;
}


// TODO: actually modify
int modify_history_stats( FILE *fp, struct run_stats stats ) {
    // extract information
    // we may or may not have just written to the file, so idk where fp is
    fseek(fp, 0, SEEK_SET); // https://stackoverflow.com/questions/32366665/resetting-pointer-to-the-start-of-file

    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    long pos;

    int old_min, old_max, old_total, old_average; // placeholder for 'min/max' value

    // loop through lines, if it has a colon, and if has 'Total', 'Highest', etc.. read respective stat
    // inspiration from https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    while ( (read = getline(&line, &len, fp)) != -1 ) {
        pos = ftell(fp);
        char *colon = strchr(line, ':');
        if ( colon ) {
            char *info = colon + 2; // we know the format is line: number (add two for ": ")
            if ( strstr(line, "Total") ) {
                old_total = strtol(info, NULL, 10);
                stats.total_shots += old_total;
            } else if ( strstr(line, "Highest") ) {
                old_max = strtol(info, NULL, 10);
                if ( old_max >  stats.max_num_shots ) {
                    stats.max_num_shots = old_max;
                }
            } else if ( strstr(line, "Fewest") ) {
                old_min = strtol(info, NULL, 10);
                if ( ( old_min != -1 ) && ( old_min < stats.min_num_shots ) ) { // -1 used instead of intmax
                    stats.min_num_shots = old_min;
                }
            } else if ( strstr(line, "Average") ) {
                // TODO: balance two averages
                old_average = strtod(info, NULL);
            }
        }
    }
    return 0;
}


int write_history( struct run_stats stats ) {
    // return 0 for success, non zero return for some error
    FILE *fp = get_history_file(); // this will set all of these values
    if ( fp == NULL ) {
        return 1;
    } else if ( insert_latest_run(fp, stats) == 1 ) {
        return 1;
    } else if ( modify_history_stats(fp, stats) == 1 ) {
        return 1;
    } else if ( print_file_information(HISTORY_FILENAME) == 1 ) {
        return 1;
    }
    printf("History written to %s\n", HISTORY_FILENAME);
    fclose(fp);
    return 0;
}
