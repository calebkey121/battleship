#include "battleship.h"


bool fire( char board[BOARD_LENGTH][BOARD_LENGTH + 1], int target_coords[2], FILE *fp ) {
    if ( target_coords[0] < 0 || target_coords[0] > BOARD_LENGTH - 1 || target_coords[1] < 0 || target_coords[1] > BOARD_LENGTH - 1 ) {
        output(fp, stdout, "target coordinates are outside of range :(\n");
        return false;
    }
    if ( board[target_coords[0]][target_coords[1]] == HORIZONTAL_CHAR || board[target_coords[0]][target_coords[1]] == VERTICAL_CHAR ) {
        board[target_coords[0]][target_coords[1]] = HIT_CHAR; // better make sure your input is clean
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "hit!\n");
        #endif
        return true;
    } else if ( board[target_coords[0]][target_coords[1]] == HIT_CHAR ) { // or miss char too, right?
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "You already hit there previously.. are you even paying attention!?!\n");
        #endif
        return false;
    } else {
        board[target_coords[0]][target_coords[1]] = MISS_CHAR; // better make sure your input is clean
        #ifndef RANDOM_ENABLED
            output(fp, stdout, "miss...\n");
        #endif
        return false;
    }
}


int game_loop( char board[BOARD_LENGTH][BOARD_LENGTH + 1], int num_hits, int game_num, FILE *fp ) {
    bool game_over = false;
    int target_coords[2];
    char input[BUFFER_SIZE];
    int hits = 0;
    int shots = 0;

    #ifdef RANDOM_ENABLED
        bool used[BOARD_LENGTH][BOARD_LENGTH] = { false }; // simple enough
        srand(time(NULL) + game_num); // add game num to make sure different seed
    #endif

    while ( hits < num_hits ) {
        #ifdef RANDOM_ENABLED
            bool valid_coords = false;
            while ( !valid_coords ) { // generate coords until one is valid
                // statistically unlikely to go infinite 
                target_coords[0] = rand() % BOARD_LENGTH;
                target_coords[1] = rand() % BOARD_LENGTH;
                if ( !used[target_coords[0]][target_coords[1]] ) {
                    valid_coords = true;
                }
            }
        #else
            if ( shots != 0 ) {
                print_board(board, NULL);
            }
            printf("Target x coord (0-%d): ", BOARD_LENGTH - 1);
            fgets(input, BUFFER_SIZE, stdin);
            while ( sscanf(input, "%d", &target_coords[0]) != 1 || target_coords[0] < 0 || target_coords[0] > BOARD_LENGTH - 1 ) {
                printf("Invalid input. Enter a number (0-%d): ", BOARD_LENGTH - 1);
                fgets(input, BUFFER_SIZE, stdin);
            }
            printf("Target y coord (0-%d): ", BOARD_LENGTH - 1);
            fgets(input, BUFFER_SIZE, stdin);
            while ( sscanf(input, "%d", &target_coords[1]) != 1 || target_coords[1] < 0 || target_coords[1] > BOARD_LENGTH - 1 ) {
                printf("Invalid input. Enter a number (0-%d): ", BOARD_LENGTH - 1);
                fgets(input, BUFFER_SIZE, stdin);
            }
            output(fp, stdout, "Targeting (%d, %d): ", target_coords[0], target_coords[1]);
        #endif

        if ( fire(board, target_coords, fp) ) {
            hits += 1;
        }
        shots++;
    }
    return shots;
}


int run_loop( char board[BOARD_LENGTH][BOARD_LENGTH + 1], struct boat b, FILE *fp ) {
    // game loop, return int 0 for success, non-zero for error
    #ifdef RANDOM_ENABLED
        int arr[NUM_RUNS];
        struct run_stats stats = { NUM_RUNS, arr, 0, 0, INT_MAX, 0.0}; //
        int shots = 0; // useful placeholder
        for ( int i = 0; i < NUM_RUNS; i++ ) {
            output(fp, stdout, ">>>--- Start Run ---<<<\n\n");
            shots = game_loop(board, b.length, i, fp);
            print_board(board, fp);
            output(fp, stdout, "you win! number of shots needed: %d\n\n", shots);
            output(fp, stdout, ">>>---  End Run  ---<<<\n");

            stats.shots_per_run[i] = shots;
            stats.total_shots += shots;
            if ( shots > stats.max_num_shots ) {
                stats.max_num_shots = shots;
            }
            if (shots < stats.min_num_shots ) {
                stats.min_num_shots = shots;
            }

            // gotta reset board each game
            set_board(board, b, fp); // don't really need to check if it was set (already did)
        }
        stats.average_num_shots = (float)stats.total_shots / NUM_RUNS; 
        
        output(fp, stdout, ">>>--- Start Summary ---<<<\n\n");
        output(fp, stdout, "Total number of shots: %d\n", stats.total_shots);
        output(fp, stdout, "Highest shots in single game: %d\n", stats.max_num_shots);
        output(fp, stdout, "Fewest shots in single game: %d\n", stats.min_num_shots);
        output(fp, stdout, "Average number of shots: %f\n\n", stats.average_num_shots);
        output(fp, stdout, ">>>---  End Summary  ---<<<\n");
        if ( write_history(stats) == 1 ) {
            output(fp, stderr, "Error writing summary file\n");
            return 1;
        }
    #else
        output(fp, stdout, ">>>--- Start Game ---<<<\n\n");
        print_board(board, fp);
        int num_shots = game_loop(board, b.length, 0, fp);
        print_board(board, fp);
        output(fp, stdout, "you win! number of shots needed: %d\n\n", num_shots);
        output(fp, stdout, ">>>--- End Game ---<<<\n");
    #endif
    return 0;
}


int main(int argc, char *argv[]) {
    int start_x = 2, start_y = 3, length = 4;
    bool horizontal = false;
    char *env_var;
    FILE *fp = fopen(LAST_RUN_FILENAME, "w"); // expected to run from a build folder inside of 6_x dir
    if ( fp == NULL ) {
        return end_game_early(fp, "Error opening file: %s\n", LAST_RUN_FILENAME, strerror(errno));
    }

    if ( argc == 1 ) {
        printf("No command line arguments passed, checking environment variables for default values...\n");
        env_var = getenv("START_X");
        if ( env_var == NULL ) {
            printf("START_X environment variable not set, using default value...\n");
        } else {
            start_x = atoi(env_var);
        } // non integer env var will just set to zero
        
        env_var = getenv("START_Y");
        if ( env_var == NULL ) {
            printf("START_Y environment variable not set, using default value...\n");
        } else {
            start_y = atoi(env_var);
        }
        
        env_var = getenv("LENGTH");
        if ( env_var == NULL ) {
            printf("LENGTH environment variable not set, using default value...\n");
        } else {
            length = atoi(env_var);
        }

        env_var = getenv("HORIZONTAL");
        if ( env_var == NULL ) {
            printf("HORIZONTAL environment variable not set, using default value...\n");
        } else {
            horizontal = *env_var != 'n';
        }

    } else if ( argc == 5 ) {
        start_x = atoi(argv[1]); // invalid input will just return 0 for start_x/y
        start_y = atoi(argv[2]);
        length = atoi(argv[3]);
        horizontal = *argv[4] != 'n'; // really this is 'n' any input that starts with n is vertical, anything else for horizontal
    }
    else {
        return end_game_early(fp, "Usage: %s start_x start_y length horizontal(y/n) // expected to run from build folder inside 6_x\n", argv[0]);
    }

    if ( length < 1 ) {
        return end_game_early(fp, "Length must be at least 1\nUsage: %s start_x start_y length horizontal(y/n)\n", argv[0]);
    }
    struct boat my_boat = { { start_x, start_y }, length, horizontal };

    char board[BOARD_LENGTH][BOARD_LENGTH + 1];
    bool set = set_board(board, my_boat, fp);
    if ( !set ) {
        return end_game_early(fp, "Error setting board\n");
    }

    int status = run_loop(board, my_boat, fp);
    fclose(fp);
    return status;
    }
