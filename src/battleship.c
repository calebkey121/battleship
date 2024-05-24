#include "battleship.h"


/**
 *  Run a game with rng selecting the target coordinates until it sinks all boats (if RANDOM_ENABLED is defined in common.h)
 * 
 *  Description:
 *      Use 2d bool array to track previously targeted coords. Randomly generates coords to target until it finally sinks the boat
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      num_hits - how many hits needed to have sunk the boat (aka length of boat)
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      shots - number of shots needed to sink the boat, used by caller to count stats
 */
int random_game_loop( Board *game_board, int total_hits_required, int game_num, FILE *fp ) {
    Coords target_coords;
    int shots = 0, hits = 0;
    int hit_coords[total_hits_required][2]; // store hits so we can have heuristics for targeting 

    bool used[BOARD_HEIGHT][BOARD_WIDTH] = { false }; // higher space complexity, but lets us go fast!
    srand(time(NULL) + game_num); // add game num to make sure different seed

    while ( hits < total_hits_required ) {
        bool valid_coords = false;
        while ( !valid_coords ) { // generate coords until one is valid
            // statistically unlikely to go infinite 
            target_coords.x = rand_number_inclusive(1, BOARD_WIDTH); // 1 indexed for user understanding, fire function will convert to 0 indexed
            target_coords.y = rand_number_inclusive(1, BOARD_HEIGHT); // 1 indexed for user understanding, fire function will convert to 0 indexed
            if ( !used[target_coords.y - 1][target_coords.x - 1] ) { // convert to 0 indexed
                valid_coords = true;
                used[target_coords.y - 1][target_coords.x - 1] = true;
            }
        }

        if ( fire(game_board, target_coords, fp) ) {
            //hit_coords[hits] = target_coords;
            hits += 1;
        }
        shots++;
    }
    return shots;
}


/**
 *  Function to run a bunch of random games, called a run (if RANDOM_ENABLED is defined in common.h)
 * 
 *  Description:
 *      Loop through number of games, run a random game, update stats, write out stats to files
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      boat - needed to reset the board each time. after a game runs the board will have been used, so needs to reset
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      int - 0 for successful game, else 1
 */
int random_run( Board *game_board, Boat *boats, int num_boats, int total_hits_required, FILE *fp ) {
    // game loop, return int 0 for success, non-zero for error
    int arr[MAX_ALLOWED_GAMES]; // make sure this can't be too big for the stack, or put it on heap
    struct run_stats stats = { MAX_ALLOWED_GAMES, arr, 0, 0, INT_MAX, 0.0}; //
    int shots = 0; // useful placeholder
    for ( int i = 0; i < MAX_ALLOWED_GAMES; i++ ) {
        output(fp, stdout, ">>>--- Start Game ---<<<\n\n");
        shots = random_game_loop(game_board, total_hits_required, i, fp);
        print_board(game_board, fp);
        output(fp, stdout, "Total number of shots: %d\n\n", shots);
        output(fp, stdout, ">>>---  End Game  ---<<<\n");

        // update stats
        stats.shots_per_game[i] = shots;
        stats.total_shots += shots;
        if ( shots > stats.highest_shots ) {
            stats.highest_shots = shots;
        }
        if (shots < stats.fewest_shots ) {
            stats.fewest_shots = shots;
        }

        // gotta reset board each game
        set_board(game_board, boats, num_boats, fp); // don't really need to check if it was set (already did)
    }
    stats.average_num_shots = (float)stats.total_shots / MAX_ALLOWED_GAMES; 
    
    // Summary results
    output(fp, stdout, ">>>--- Start Summary ---<<<\n\n");
    output(fp, stdout, "Total number of shots: %d\n", stats.total_shots);
    output(fp, stdout, "Highest shots in single game: %d\n", stats.highest_shots);
    output(fp, stdout, "Fewest shots in single game: %d\n", stats.fewest_shots);
    output(fp, stdout, "Average number of shots: %f\n\n", stats.average_num_shots);
    output(fp, stdout, ">>>---  End Summary  ---<<<\n");
    
    // Update history file
    if ( write_history(stats) == 1 ) {
        output(fp, stderr, "Error writing summary file\n");
        return 1;
    }
    return 0;
}


/**
 *  Game loop to be run when user manually plays the game (if RANDOM_ENABLED is not defined in common.h)
 * 
 *  Description:
 *      Gets target coordinates from user, fire those coordinates, repeat until you sink the boat
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      num_hits - How many hits it takes to sink all of the boats on the board. aka the length of the boat
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      int - 0 for successful game, else 1
 * 
 *  TODO:
 *      Let a player quit in the middle of game?
 *      Change targeting system to be more like the actual battleship game 
 */
int manual_game_loop( Board *game_board, int total_hits_required, FILE *fp ) {
    Coords target_coords;
    int hits = 0, shots = 0, buffer_size = 10;
    char input[buffer_size];

    output(fp, stdout, ">>>--- Start Game ---<<<\n\n");
    while ( hits < total_hits_required ) { // loop until we get the number of hits expected
        // todo: should we consider letting the player quit in the middle of a game?
        print_board(game_board, NULL);
        printf("Target x coord (1-%d): ", BOARD_WIDTH); // 1 indexed for user understanding, fire function will convert to 0 indexed
        fgets(input, buffer_size, stdin);
        while ( sscanf(input, "%d", &target_coords.x) != 1 || target_coords.x < 1 || target_coords.x > BOARD_WIDTH ) {
            printf("Invalid input. Enter a number (1-%d): ", BOARD_WIDTH);
            fgets(input, buffer_size, stdin);
        }
        printf("Target y coord (1-%d): ", BOARD_HEIGHT); // 1 indexed for user understanding, fire function will convert to 0 indexed
        fgets(input, buffer_size, stdin);
        while ( sscanf(input, "%d", &target_coords.y) != 1 || target_coords.y < 1 || target_coords.y > BOARD_HEIGHT ) {
            printf("Invalid input. Enter a number (1-%d): ", BOARD_HEIGHT);
            fgets(input, buffer_size, stdin);
        }
        output(fp, stdout, "Targeting (%d, %d): ", target_coords.x, target_coords.y);

        if ( fire(game_board, target_coords, fp) ) {
            hits += 1;
        }
        shots++;
    }

    print_board(game_board, fp);
    output(fp, stdout, "you win! number of shots needed: %d\n\n", shots);
    output(fp, stdout, ">>>--- End Game ---<<<\n");
    return 0;
}


/**
 *  Place boat on our board: set boat coords to boat and non-boat coords to water
 * 
 *  Description:
 *      Set every coord to empty (water), double check boat fits on board, then set boat coords to boat char (horizontal/vertical)
 * 
 *  Parameters:
 *      board - 2d char array that holds the value (boat or water) for each coordinate
 *      boat - to be placed on the board. It's coords/length are used to check that it fits, and to place it.
 *      fp - file pointer for the current run results. Write the stdout to this file as well
 * 
 *  Return:
 *      bool - true if successfully placed, false otherwise
 */
bool set_board( Board *game_board, Boat *boats, int num_boats, FILE *fp ) {
    for ( int i = 0; i < BOARD_HEIGHT; i++ ) {
        for ( int j = 0; j < BOARD_WIDTH; j++ ) {
            game_board->board[i][j] = EMPTY_CHAR;
        } 
        game_board->board[i][BOARD_WIDTH] = '\0';
    }

    if ( !boats_fit_on_board(boats, num_boats) ) {
        output(fp, stdout, "boats do not fit on board :(\n");
        return false;
    }
    
    for ( int i = 0; i < num_boats; i++ ) {
        Boat b = boats[i];
        int start_x = b.start_coords.x, start_y = b.start_coords.y;
        int y_offset = b.horizontal ? 1 : b.length; // needed to find to correct row to place boat
        char *curr = &game_board->board[BOARD_HEIGHT - start_y - y_offset][start_x]; 
        for ( int i = 0; i < b.length; i++ ) {
            if ( b.horizontal ) {
                if ( i == 0 ) {
                    *curr = HORIZONTAL_LEFT_CHAR;// ? ( b.length > 1 ) : HORIZONTAL_CHAR;
                } else if ( i == b.length - 1 ) {
                    *curr = HORIZONTAL_RIGHT_CHAR;
                } else {
                    *curr = HORIZONTAL_CHAR;
                }
                curr += 1;
            } else { // vertical - increase by length of board
                if ( i == 0 ) {
                    *curr = VERTICAL_UP_CHAR;// ? ( b.length > 1 ) : VERTICAL_CHAR;
                } else if ( i == b.length - 1 ) {
                    *curr = VERTICAL_DOWN_CHAR;
                } else {
                    *curr = VERTICAL_CHAR;
                }
                curr += BOARD_WIDTH + 1;
            }
        }
    }
    
    return true;
}


/**
 * Program that lets the user play battleship, either manually or letting the computer randomly play and watch 
 * 
 * 
*/
int main(int argc, char *argv[]) {
    srand(time(NULL));
    // Delete old run file (pretty silly, we could just open with 'w', but jqr says explicitly to delete a file)
    if ( file_exists(LAST_RUN_FILENAME) ) {
        if ( remove(LAST_RUN_FILENAME) != 0 ) {
            perror("Error Deleting File");
            return 1;
        }
    }

    // Create new run file
    FILE *fp = fopen("hello.txt", "w"); // expected to run from a build folder inside of 6_x dir
    if ( fp == NULL ) {
        return end_game_early(fp, "Error opening file: %s\n", LAST_RUN_FILENAME, strerror(errno));
    }

    // input for boat, this was required before, but now we're just going to hard code it
    /*  
    // Get different input methods (env vars, command line arguments)
    int start_x = 2, start_y = 3, length = 4;
    bool horizontal = false;
    char *battleship;
    if ( argc == 1 ) {
        if ( setenv("BATTLESHIP", "hello", 1) != 0 ) {
            return end_game_early(fp, "Error setting environment variable");
        }
        get_battleship_env(&start_x, &start_y, &length, &horizontal);
        if ( ( battleship = getenv("BATTLESHIP") ) == NULL ) {
            return end_game_early(fp, "Error retrieving environment variable");
        } else {
            printf("BATTLESHIP env var: %s\n", battleship);
        }
    } else if ( argc == 5 ) {
        // TODO more robust function to gather command line arguments
        start_x = atoi(argv[1]) - 1; // invalid input will just return 0 // 1 indexed
        start_y = atoi(argv[2]) - 1; // 1 indexed
        length = atoi(argv[3]);
        horizontal = *argv[4] != 'n'; // really this is 'n' any input that starts with n is vertical, anything else for horizontal
    }
    else {
        return end_game_early(fp, "Usage: %s start_x start_y length horizontal(y/n)\n", argv[0]);
    }

    // create boat from input
    // todo: many boats?
    if ( length < 1 ) {
        return end_game_early(fp, "Length must be at least 1\nUsage: %s start_x start_y length horizontal(y/n)\n", argv[0]);
    }
    */
    int *num_boats;
    int *total_hits_required;
    Boat* boats = make_random_boats(num_boats, total_hits_required);
    Boat boat1 = boats[0];
    // Set board
    Board *game_board = malloc(sizeof(Board));
    if (game_board == NULL) {
        return end_game_early(fp, "Error allocating memory for game board\n");
    }
    bool set = set_board(game_board, boats, *num_boats, fp);
    if ( !set ) {
        return end_game_early(fp, "Error setting board\n");
    }

    // Start run
    #ifdef RANDOM_ENABLED
        int status = random_run(game_board, boats, *num_boats, *total_hits_required, fp);
    #else
        int status = manual_game_loop(game_board, *total_hits_required, fp);
    #endif

    fclose(fp);
    return status;
}
