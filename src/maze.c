#include "maze.h"
#include <stdlib.h>
#include <memory.h>

struct maze*
maze_new ( int w, int h ) {
	struct maze *m;
	m = malloc(sizeof(struct maze));
	maze_init( m, w, h );
	return m;
}

void
maze_init ( struct maze *m, int w, int h ) {
	if(m) {
		m->w = w;
		m->h = h;
		m->l = w*h;
		memset( m->cells, 0, sizeof(Cell)*m->l );
	}
}

void
maze_free ( struct maze *m ) {
	if(m) {
		free(m);
	}
}