#include "recurse.h"
#include <stdlib.h>
#include <memory.h>

struct chamber {	
	int x1,y1,x2,y2;
};

int g_len;
struct chamber g_chambers[MAZE_MAX_SIZE];

void
recurse_init( struct maze *m ) {
	memset( m->cells, WALLS, sizeof(Cell)*m->l );
	struct chamber c = { .x1=0,.y1=0,.x2=m->w,.y2=m->h };
	g_chambers[0] = c;
	g_len = 1;
}

void
recurse_gen( struct maze *m ) {
	recurse_init(m);
	recurse_step(m,-1);
}

void
recurse_step( struct maze *m, int n ) {
	int w,h,xn,yn,i,j;
	struct chamber c;

	while(n && g_len) {
		c = g_chambers[g_len-1];
		g_len--;
		if( (c.x2-c.x1)>1 && (c.y2-c.y1)>1 ) {
			w = c.x2-c.x1;
			h = c.y2-c.y1;
			xn = c.x1 + rand()%(w-1) + 1;
			yn = c.y1 + rand()%(h-1) + 1;

			for(i=c.x1; i<c.x2-1; i++) {
				m->cells[i+yn*m->w] |= CELL_IN_MAZE;
				m->cells[i+yn*m->w] &= ~WALL_EAST;
				m->cells[(i+1)+yn*m->w] &= ~WALL_WEST;
			}

			for(i=c.y1; i<c.y2-1; i++) {
				m->cells[xn+i*m->w] |= CELL_IN_MAZE;
				m->cells[xn+i*m->w] &= ~WALL_SOUTH;
				m->cells[xn+(i+1)*m->w] &= ~WALL_NORTH;
			}

			struct chamber q[] = {
				{ .x1=c.x1,.y1=c.y1,.x2=xn,.y2=yn },
				{ .x1=c.x1,.y1=yn,.x2=xn,.y2=c.y2 },
				{ .x1=xn,.y1=c.y1,.x2=c.x2,.y2=yn },
				{ .x1=xn,.y1=yn,.x2=c.x2,.y2=c.y2 }
			};

			// randomly create an opening in three of the four quadrants we created
			for( i=0; i<3; i++ ) {
				j = rand()%(4-i);
				c = q[j];

				q[j] = q[3-i];
				q[3-i] = c;
			}
			
			for( i=0; i<4; i++ ) {
				g_chambers[g_len++]=q[i];
			}

			n--;
		}				
	}
}