#ifndef RECURSE_H
#define RECURSE_H

#include "maze.h"

void recurse_init( struct maze *m );
void recurse_gen( struct maze *m );
void recurse_step( struct maze *m, int n );

#endif