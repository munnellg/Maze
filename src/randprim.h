#ifndef RANDPRIM_H
#define RANDPRIM_H

#include "maze.h"

void randprim_init( struct maze *m );
void randprim_gen( struct maze *m );
void randprim_step( struct maze *m, int n );

void randprim_cell_init( struct maze *m );
void randprim_cell_gen( struct maze *m );
void randprim_cell_step( struct maze *m, int n );

void dfs_init( struct maze *m );
void dfs_gen( struct maze *m );
void dfs_step( struct maze *m, int n );

#endif