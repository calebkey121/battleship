#ifndef BOAT_H
#define BOAT_H

#include <stdbool.h>
#include "common.h"
#include "battleship_tools.h"

// forward declaration
bool on_board(int x, int y);

typedef struct {
    Coords start_coords;
    int length;
    bool horizontal; // orientation, true for horizontal, false for vertical
} Boat;

bool boat_fits_on_board( Boat b );
bool boats_fit_on_board( Boat *boats, int num_boats );
bool two_boats_collide( Boat b1, Boat b2 );
bool boats_collide( Boat *boats, int num_boats );
Boat* make_random_boats( int *num_boats, int *total_hits_required );

#endif
