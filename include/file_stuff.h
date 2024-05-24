// This file defines file utilities used in battleship

#ifndef FILE_STUFF_H
#define FILE_STUFF_H

// make sure all of these are needed
#include <stdio.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>

#include "common.h"

void output( FILE *file, FILE *console, const char *formatstring, ... );
bool file_exists( const char *filename);
FILE *get_history_file();
int write_history( struct run_stats stats );
int insert_latest_run( FILE *fp, struct run_stats stats );
int modify_history_stats( FILE *fp, struct run_stats stats );
int print_file_information( const char *filename );

#endif
