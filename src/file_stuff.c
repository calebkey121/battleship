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


// refresher used - https://stackoverflow.com/questions/73978260/va-list-in-c-creating-a-function-that-doesnt-need-a-argument-count-like-print
/**
 *  print to both file and console (stdout, stderr) with a format string like printf
 *  useful because now we don't have to print twice everytime we have any output
 * 
 *  Description:
 *      Use stdarg.t to have variable arguments, initialize the va_list type to hold the addl. args, output to both file pointers
 * 
 *  Parameters:
 *      file - first file pointer to that we want to print to 
 *      console - second file pointer to that we want to print to 
 *      formatstring ..., the string that we're outputting and the additional arguments we might want to format for 
 * 
 *  Return:
 *      void
 */
void output( FILE *file, FILE *console, const char *formatstring, ... ) {
    va_list args;

    va_start(args, formatstring);
    vfprintf(file, formatstring, args);
    va_end(args);

    va_start(args, formatstring);
    vfprintf(console, formatstring, args);
    va_end(args); 
}


/**
 *  Simple function to check if a file exists or not
 * 
 *  Description:
 *      access function from unistd.h will return 0 if file exists
 * 
 *  Parameters:
 *      filename - path to file we want to check if it exists or not
 * 
 *  Return:
 *      bool - true if the file exists, else false
 */
bool file_exists( const char *filename ) {
    // refresher used - https://stackoverflow.com/questions/230062/whats-the-best-way-to-check-if-a-file-exists-in-c
    if (access(filename, F_OK) == 0) {
        // file exists
        return true;
    } else {
        // file doesn't exist
        return false;
    }
}


/**
 *  Function to return the history file pointer, and creates/initializes it if it doesn't exist
 * 
 *  Description:
 *      Checks if the file exists: if it doesn't then create it (mode a+) and initialize base information
 *          Then open file in r+ mode and return file pointer 
 * 
 *  Parameters:
 *      None (assume filename exists in 'HISTORY_FILENAME')
 * 
 *  Return:
 *      fp - file pointer for history file in r+ mode
 */
FILE *get_history_file() {
    // we want this open in r+, but we need to make it if it doesn't exist
    FILE *fp;
    if ( !file_exists(HISTORY_FILENAME) ) { // write base information
        fp = fopen(HISTORY_FILENAME, "a+");
        if ( fp == NULL ) {
                return NULL;
        }
        fprintf(fp, ">>>--- Start History ---<<<\n\n"\
                    "\n>>>---  End History  ---<<<\n"\
                    ">>>--- Start History Stats ---<<<\n\n"\
                    "Total number of shots: 0\n"\
                    "Highest shots in single game: 0\n"\
                    "Fewest shots in single game: -1\n"\
                    "Average number of shots: 0\n"\
                    "\n>>>---  End History Stats  ---<<<\n");
        fclose(fp);
    }
    fp = fopen(HISTORY_FILENAME, "r+");
    if ( fp == NULL ) {
            return NULL;
    }
    return fp;
}


/**
 *  Insert a line into history file with details about the latest run (stats)
 * 
 *  Description:
 *      Move fp to insertion point, read rest of file into buffer, write new line, write rest of file
 *      Assume caller handles open/close of fp
 * 
 *  Parameters:
 *      fp - file we're inserting into
 *      stats - the numbers we're updating the file with
 * 
 *  Return:
 *      int - 0 for success, else 1
 * 
 *  TODO:
 *      Currently we assume we want to insert at line 2: make this custom or find a better spot to insert
 */
int insert_latest_run( FILE *fp, struct run_stats stats ) {
    fseek(fp, 0, SEEK_SET);
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    long insertion;
    char buffer[1024];

    int skip = 2; // skip first two lines to get to insertion point
    for ( int i = 0; i < skip; i++ ) {
        if ( (getline(&line, &len, fp)) == -1 ) {
            return 1;
        }
    }
    insertion = ftell(fp);

    // read rest of file into a buffer
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[bytes_read] = '\0';

    fseek(fp, insertion, SEEK_SET);

    // write back new line and old data
    char insert_line[100]; // make sure given MAX_GAMES_ALLOWED, the lines don't get longer than this buffer, or change the buffer
    sprintf(insert_line, "Number of Games: %d, Total Shots: %d, Average Shots per Run: %f\n", stats.num_games, stats.total_shots, stats.average_num_shots);
    fputs(insert_line, fp);
    fputs(buffer, fp);

    return 0;
}


// refresher used - https://codeforwin.org/c-programming/c-program-find-file-properties-using-stat-function
/**
 *  Print relevant file information, enough to cover jqr item 6.8
 * 
 *  Description:
 *      Grab the file stats and print each of them
 *      Assume caller handles opening/closing
 * 
 *  Parameters:
 *      filename - the name of the file we are printing stats of
 * 
 *  Return:
 *      int - 0 for success, else 1
 */
int print_file_information( const char *filename ) {
    struct stat stats;
    if ( stat(filename, &stats) == 0 ) {
        printf("File type & permissions: %d\n", stats.st_mode);
        printf("File size: %lld bytes\n", stats.st_size);
        printf("Inode: %llu bytes\n", stats.st_ino);
        printf("Device: %d\n", stats.st_dev);
        printf("User ID: %d\n", stats.st_uid);
        printf("Group ID: %d\n", stats.st_gid);
        printf("Last Access Time: %ld\n", stats.st_atime);
        printf("Last Modification Time: %ld\n", stats.st_mtime);
        printf("Number of Blocks: %lld\n", stats.st_blocks);        
    } else {
        // Failed to get file information
        return 1;
    }
    return 0;
}


/**
 *  We have a running total of stats in history file. Update those stats by modifying the file
 * 
 *  Description:
 *      Read Through lines, if the line matches its cooresponding stat, update that stat
 * 
 *  Parameters:
 *      fp - file we're modifying (history.txt)
 *      stats - the numbers we're updating the file with
 * 
 *  Return:
 *      int - 0 on success, else 1
 * 
 *  TODO:
 *      Actually modify
 */
int modify_history_stats( FILE *fp, struct run_stats stats ) {
    // we may or may not have just written to the file, so idk where fp is
    fseek(fp, 0, SEEK_SET);

    char *line = NULL, *info = NULL;
    size_t len = 0;
    ssize_t read;
    int old_min, old_max, old_total, old_average; // placeholder for 'min/max' value

    // loop through lines, if it has a colon, and if has 'Total', 'Highest', etc.. read respective stat
    // inspiration from https://stackoverflow.com/questions/3501338/c-read-file-line-by-line
    while ( (read = getline(&line, &len, fp)) != -1 ) {
        if ( (info = strstr(line, "Total number of shots: ")) != NULL ) {
            info += 23; // move to end of line
            old_total = strtol(info, NULL, 10);
            stats.total_shots += old_total;
        } else if ( (info = strstr(line, "Highest shots in single game: ")) != NULL ) {
            info += 30; // move to end of line
            old_max = strtol(info, NULL, 10);
            if ( old_max >  stats.highest_shots ) {
                stats.highest_shots = old_max;
            }
        } else if ( (info = strstr(line, "Fewest shots in single game: ")) != NULL ) {
            info += 29; // move to end of line
            old_min = strtol(info, NULL, 10);
            if ( ( old_min != -1 ) && ( old_min < stats.fewest_shots ) ) { // -1 used instead of intmax
                stats.fewest_shots = old_min;
            }
        } else if ( (info = strstr(line, "Average number of shots: ")) != NULL ) {
            // TODO: balance two averages
            info += 25; // move to end of line
            old_average = strtod(info, NULL);
        }
    }
    return 0;
}


/**
 *  Does everything needed to create/write history file
 * 
 *  Description:
 *      Get the file pointer for history file, insert latest run, modify stats, and print file information
 * 
 *  Parameters:
 *      stats - The numbers that we need to update the history file with
 * 
 *  Return:
 *      int - 0 for success, else 1
 */
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
