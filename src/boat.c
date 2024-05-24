#include "boat.h"

/**
 *  Check if a boat fits on the board
 * 
 *  Description:
 *      Check if the boat fits on the board by checking if the start coords are on the board
 * 
 *  Parameters:
 *      b - boat to check if it fits on the board
 * 
 *  Return:
 *      bool - true if the boat fits on the board, false otherwise
 **/
bool boat_fits_on_board( Boat b ) {
    bool starts_on_board = on_board(b.start_coords.x, b.start_coords.y);
    bool ends_on_board =  b.horizontal ? on_board(b.start_coords.x + b.length, b.start_coords.y) : on_board(b.start_coords.x, b.start_coords.y + b.length);
    return starts_on_board && ends_on_board;
}


/**
 *  Check if all boats fit on the board
 * 
 *  Description:
 *      Check if all boats fit on the board by checking if the start coords are on the board
 * 
 *  Parameters:
 *      boats - array of boats to check if they fit on the board
 *      num_boats - number of boats in the array
 * 
 *  Return:
 *      bool - true if all boats fit on the board, false otherwise
 **/
bool boats_fit_on_board( Boat *boats, int num_boats ) {
    for ( int i = 0; i < num_boats; i++ ) {
        if ( !boat_fits_on_board(boats[i]) ) {
            return false;
        }
    }
    return true;
}


/**
 *  Check if two boats collide
 * 
 *  Description:
 *      Check if two boats collide by checking if any of the coords overlap
 * 
 *  Parameters:
 *      b1 - first boat to check for collision
 *      b2 - second boat to check for collision
 * 
 *  Return:
 *      bool - true if the boats collide, false otherwise
 **/
bool two_boats_collide( Boat b1, Boat b2 ) {
    int b1_x = b1.start_coords.x, b1_y = b1.start_coords.y;
    int b2_x = b2.start_coords.x, b2_y = b2.start_coords.y;
    // quick check if the boats are on the same point
    if ( b1_x == b2_x && b1_y == b2_y ) {
        return true;
    }
    if ( b1.horizontal && b2.horizontal ) {
        if ( b1_y != b2_y ) {
            return false;
        }
        Boat left_boat = b1_x < b2_x ? b1 : b2;
        Boat right_boat = b1_x < b2_x ? b2 : b1;
        return left_boat.start_coords.x + left_boat.length > right_boat.start_coords.x;
    } else if ( !b1.horizontal && !b2.horizontal ) {
        if ( b1_x != b2_x ) {
            return false;
        }
        Boat bottom_boat = b1_y < b2_y ? b1 : b2;
        Boat top_boat = b1_y < b2_y ? b2 : b1;
        return top_boat.start_coords.y + top_boat.length > bottom_boat.start_coords.y;
    } else {
        Boat horizontal_boat = b1.horizontal ? b1 : b2;
        Boat vertical_boat = b1.horizontal ? b2 : b1;
        // boats collide IF vertical boat's x coord is in range of horizontal boat's x coord and length
        //      AND horizontal boat's y coord is in range of vertical boat's y coord and length
        // Check if vertical boat's x coord is in range of horizontal boat's x coord and length
        bool x_in_range = is_in_range(vertical_boat.start_coords.x, horizontal_boat.start_coords.x, horizontal_boat.start_coords.x + horizontal_boat.length);
        // Check if horizontal boat's y coord is in range of vertical boat's y coord and length
        bool y_in_range = is_in_range(horizontal_boat.start_coords.y, vertical_boat.start_coords.y, vertical_boat.start_coords.y + vertical_boat.length);
        return x_in_range && y_in_range;
    }
}


/**
 *  Create a random set of boats to be placed on the board
 * 
 *  Description:
 *      Create a random number of boats with random lengths and random orientations
 * 
 *  Parameters:
 *      num_boats - number of boats to create, will be capped at MAX_BOATS
 *      max_length - maximum length of a boat, will be capped at MAX_BOAT_LENGTH
 * 
 *  Return:
 *      Boat* - pointer to the array of boats created
 **/
Boat* make_random_boats( int *num_boats, int *total_hits_required ) {
    Boat *boats = (Boat*)malloc(NUM_BOATS * sizeof(Boat));
    bool boat_fits = false;
    int max_retries = 1000;
    int retries = max_retries;
    *num_boats = 0; // may not be able to place all boats
    *total_hits_required = 0;
    for ( int i = 0; i < NUM_BOATS; i++ ) {
        do { // make new boats, until we get one that fits on the board and doesn't collide with any other boat
            boats[i].start_coords.x = rand_number_inclusive(0, BOARD_WIDTH - 1);
            boats[i].start_coords.y = rand_number_inclusive(0, BOARD_HEIGHT - 1);
            boats[i].length = rand_number_inclusive(MIN_BOAT_LENGTH, MAX_BOAT_LENGTH);
            boats[i].horizontal = rand_number_inclusive(0, 1);
            boat_fits = boat_fits_on_board(boats[i]) && !boats_collide(boats, i + 1);
            if (boat_fits) {
                *num_boats += 1;
                retries = max_retries;
                *total_hits_required += boats[i].length;
            } else {
                retries--;
            }
        } while ( !boat_fits && retries > 0);
        // probably won't go infinite, because we're capping the number of boats
        // with a small enough board and enough boats, there might be a chance that we can't place any more boats
        // you're guarenteed BOATD_HEIGHT * BOARD_WIDTH boats, with length of 1, but since its random it could take a while
    }
    return boats;
}



/**
 *  Check if any boats collide
 * 
 *  Description:
 *      Check if any boats collide by checking if any of the boats collide with any other boat
 * 
 *  Parameters:
 *      boats - array of boats to check for collision
 *      num_boats - number of boats in the array
 * 
 *  Return:
 *      bool - true if any boats collide, false otherwise
 **/
bool boats_collide(Boat *boats, int num_boats) {
    for (int i = 0; i < num_boats; i++) {
        for (int j = 0; j < num_boats; j++) {
            if (i != j && two_boats_collide(boats[i], boats[j])) {
                return true; // collision
            }
        }
    }
    return false; // None of the boats collide
}
