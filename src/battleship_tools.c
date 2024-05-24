#include "battleship_tools.h"

/**
 *  A few things that needs to happen if we run into an error while running the game to wrap things up
 * 
 *  Description:
 *      Write some error output to file and close file pointer
 * 
 *  Parameters:
 *      fp - file pointer that needs to be closed
 *      msg - some error message saying what went wrong
 *      ... - addl. arguments that can be formatted into msg
 * 
 *  Return:
 *      1 - since this is called on error, we will return 1 which indicates error
 */
int end_game_early( FILE *fp, const char *msg, ...) {
    va_list args;

    output(fp, stderr, msg, args);
    output(fp, stdout, "\n>>>--- End Game ---<<<\n");
    fclose(fp);
    return 1;
}


/**
 *  Grab the environment variables and set them to their respective argument pointer. 
 * 
 *  Description:
 *      Make sure env vars exists and set them to their respective variable.
 *      If the env var isn't an int, those will be set to zero
 *      If 'horizontal' env var start with an 'n' will be false, else true
 * 
 *  Parameters:
 *      pointers to the env vars we are grabbing
 * 
 *  Return:
 *      void
 * 
 *  TODO:
 *      Some validation/checking that the env vars are what we expect
 *      Make the 'horizontal' check better...
 */
void get_battleship_env( int *start_x, int *start_y, int *length, bool *horizontal ) {
    char *env_var;
    printf("No command line arguments passed, checking environment variables for default values...\n");
    if ( ( env_var = getenv("START_X") ) == NULL ) {
        printf("START_X environment variable not set, using default value...\n");
    } else {
        *start_x = atoi(env_var) - 1; // 1 indexed
    } // non integer env var will just set to zero
    
    if ( ( env_var = getenv("START_Y") ) == NULL ) {
        printf("START_Y environment variable not set, using default value...\n");
    } else {
        *start_y = atoi(env_var) - 1; // 1 indexed
    }
    
    if ( ( env_var = getenv("LENGTH") ) == NULL ) {
        printf("LENGTH environment variable not set, using default value...\n");
    } else {
        *length = atoi(env_var);
    }

    if ( ( env_var = getenv("HORIZONTAL") ) == NULL ) {
        printf("HORIZONTAL environment variable not set, using default value...\n");
    } else {
        *horizontal = *env_var != 'n';
    }
}


bool is_in_range( int x, int lower, int upper ) {
    return x >= lower && x < upper;
}


int rand_number_inclusive( int lower, int upper ) {
    return rand() % (upper - lower + 1) + lower;
}
