#ifndef MAZE_H
#define MAZE_H

#include <stdint.h>

#define WALLS         0x000F
#define WALL_NORTH    0x0001
#define WALL_WEST     0x0002
#define WALL_SOUTH    0x0004
#define WALL_EAST     0x0008
#define CELL_IN_MAZE  0x0010

#define WALL_OPPOSITE(w) ((((w)>>2)|((w)<<2))&WALLS)

#define MAZE_MAX_SIZE 0xFFFF

typedef uint8_t Cell;
struct maze {
	int w, h, l;
	Cell cells[MAZE_MAX_SIZE];
};

struct maze* maze_new( int w, int h );
void maze_init( struct maze *m, int w, int h );
void maze_free( struct maze *m );
#endif