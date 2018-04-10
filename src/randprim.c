#include "randprim.h"
#include <stdlib.h>
#include <memory.h>

struct listnode {
	uint8_t wall;
	int     idx;
};

int g_len;                             // wall list length of zero
struct listnode g_list[MAZE_MAX_SIZE]; // empty wall list

void
randprim_init( struct maze *m ) {
	int wall, ci = rand()%(m->l); // choose random starting cell

	memset( m->cells, WALLS, sizeof(Cell)*m->l );
	m->cells[ci] |= CELL_IN_MAZE; // mark cell as part of maze
	
	g_len=0;
	// add walls of cell to wall set
	for( wall=1; wall<0x10; wall<<=1 ) {
		if( wall & m->cells[ci] ) { 
			struct listnode node = { .wall=wall, .idx=ci };
			g_list[g_len++] = node; 
		}
	}
}

void
randprim_gen( struct maze *m ) {
	randprim_init(m);
	randprim_step(m, -1);
}

void
randprim_step( struct maze *m, int n ) {
	int ci,wi,ni,wall;

	while(n && g_len) {
		// select a random wall set from the wall list
		wi = rand()%g_len;
		ci = g_list[wi].idx;
		wall = g_list[wi].wall;

		// determine the neighbour on the other side of the wall
		ni = ci+(wall==WALL_EAST)-(wall==WALL_WEST)+
			m->w*(wall==WALL_SOUTH)-m->w*(wall==WALL_NORTH);

		// check bounds
		if(ni>=0 && ni<(m->l) && (ni%m->w==ci%m->w||ni/m->w==ci/m->w)) {
			// check if we've already visited the neighbouring cell
			if( !(m->cells[ni]&CELL_IN_MAZE) ) {
				// if not, form a passage between current cell and neighbour
				m->cells[ci] &= ~wall;				
				m->cells[ni] &= ~ WALL_OPPOSITE(wall);
				// mark as part of maze and add neighbour to wall set
				m->cells[ni] |= CELL_IN_MAZE;				
				for( wall=1; wall<0x10; wall<<=1 ) {
					if( wall & m->cells[ni] ) {
						struct listnode node = { .wall=wall, .idx=ni };
						g_list[g_len++] = node; 
					}
				}
				n--;			
			}
		}

		g_len--;			
		g_list[wi] = g_list[g_len];
	}
}

void
randprim_cell_init( struct maze *m ) {	
	int ci= rand()%(m->l);
	memset( m->cells, WALLS, sizeof(Cell)*m->l );
	struct listnode node = { .wall=(m->cells[ci]&WALLS), .idx=ci };
	g_list[0] = node; 	
	m->cells[ci] |= CELL_IN_MAZE;                   // mark cell as part of maze
	g_len = 1;
}

void randprim_cell_gen( struct maze *m ) {
	randprim_cell_init(m);
	randprim_cell_step(m, -1);
}

void
randprim_cell_step( struct maze *m, int n ) {
	int ci,wi,ni,ws,bit,wall;

	while(n && g_len) {
		// select a random wall set from the wall list
		wi = rand()%g_len;
		ci = g_list[wi].idx;
		ws = g_list[wi].wall;

		// select a random wall from the wall set
		wall = 0;
		bit = rand()%4;		
		while(!wall){ wall = ws&(1<<bit); bit=(bit+1)%4; }

		// determine the neighbour on the other side of the wall
		ni = ci+(wall==WALL_EAST)-(wall==WALL_WEST)+
			m->w*(wall==WALL_SOUTH)-m->w*(wall==WALL_NORTH);

		// check bounds
		if(ni>=0 && ni<(m->l) && (ni%m->w==ci%m->w||ni/m->w==ci/m->w)) {
			// check if we've already visited the neighbouring cell
			if( !(m->cells[ni]&CELL_IN_MAZE) ) {
				// if not, form a passage between current cell and neighbour
				m->cells[ci] &= ~wall;
				m->cells[ni] &= ~ WALL_OPPOSITE(wall);
				// add neighbour to wall set and mark as part of maze
				struct listnode node = { .wall=(m->cells[ni]&WALLS), .idx=ni };
				g_list[g_len++] = node; 	
				m->cells[ni] |= CELL_IN_MAZE;
				n--;
			}
		}

		g_list[wi].wall &= ~wall; // remove the wall from the set

		// if the wall set has no more walls, delete it from the list
		if(!g_list[wi].wall) {			
			g_len--;			
			g_list[wi] = g_list[g_len];
		}
	}
}

void
dfs_init( struct maze *m ) {
	int ci= rand()%(m->l);
	memset( m->cells, WALLS, sizeof(Cell)*m->l );
	struct listnode node = { .wall=(m->cells[ci]&WALLS), .idx=ci };
	g_list[0] = node; 	
	m->cells[ci] |= CELL_IN_MAZE;                   // mark cell as part of maze
	g_len = 1;
}

void
dfs_gen( struct maze *m ) {
	dfs_init(m);
	dfs_step(m,-1);
}

void
dfs_step( struct maze *m, int n ) {
	int ci,wi,ni,ws,bit,wall;

	while(n && g_len) {
		wi = g_len-1;
		ci = g_list[wi].idx;
		ws = g_list[wi].wall;

		// select a random wall from the wall set
		wall = 0;
		bit = rand()%4;		
		while(!wall){ wall = ws&(1<<bit); bit=(bit+1)%4; }

		// determine the neighbour on the other side of the wall
		ni = ci+(wall==WALL_EAST)-(wall==WALL_WEST)+
			m->w*(wall==WALL_SOUTH)-m->w*(wall==WALL_NORTH);

		// check bounds
		if(ni>=0 && ni<(m->l) && (ni%m->w==ci%m->w||ni/m->w==ci/m->w)) {
			// check if we've already visited the neighbouring cell
			if( !(m->cells[ni]&CELL_IN_MAZE) ) {
				// if not, form a passage between current cell and neighbour
				m->cells[ci] &= ~wall;
				m->cells[ni] &= ~ WALL_OPPOSITE(wall);
				// add neighbour to wall set and mark as part of maze
				struct listnode node = { .wall=(m->cells[ni]&WALLS), .idx=ni };
				g_list[g_len++] = node; 	
				m->cells[ni] |= CELL_IN_MAZE;
				n--;
			}
		}

		g_list[wi].wall &= ~wall; // remove the wall from the set

		// pop cells which have exhausted their wall set off the stack
		while(g_len && !g_list[g_len-1].wall) { g_len--; }
	}
}