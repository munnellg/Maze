#include "maze.h"
#include "randprim.h"
#include "recurse.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH  812  // weird resolution, but maze looks nicest with
#define SCREEN_HEIGHT 596  // these dimensions.
#define CELL_SIZE     4    // a maze cell is four pixels in size

// Maze will fill the window
#define MAZE_WIDTH  (SCREEN_WIDTH/(CELL_SIZE*2)) 
#define MAZE_HEIGHT (SCREEN_HEIGHT/(CELL_SIZE*2))

struct {
	void (*init)( struct maze *m );
	void (*step)( struct maze *m, int i );
	const char* name;
} g_generator[] = {
	{ 
		.init=dfs_init, 
		.step=dfs_step, 
		.name="Graph: Depth First Search" 
	},
	{ 
		.init=randprim_init, 
		.step=randprim_step, 
		.name="Graph: Randomized Prim 1" 
	},
	{ 
		.init=randprim_cell_init, 
		.step=randprim_cell_step, 
		.name="Graph: Randomized Prim 2" 
	},
	{ 
		.init=recurse_init, 
		.step=recurse_step, 
		.name="Recursive" 
	}
};

SDL_Window   *g_window;
SDL_Renderer *g_renderer;
int g_user_quit = 0, g_step=1, g_igen=0;
struct maze g_maze;

int
init_display ( void ) {    
    if( SDL_Init( SDL_INIT_VIDEO ) != 0 ) {
        /* something went wrong. Report the error and return false */
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initialize_display - SDL_Init: %s\n", 
            SDL_GetError()
        );
        return 0;
    }
   
    if( SDL_CreateWindowAndRenderer( SCREEN_WIDTH, SCREEN_HEIGHT, 0, 
                                        &g_window, &g_renderer ) < 0 ) {
        /* something went wrong. Report the error and return false */
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, 
            "initialize_display - SDL_CreateWindowAndRenderer - %s\n", 
            SDL_GetError()
        );
        return 0;
    }
   
    return 1;
}

void 
render ( Cell *maze, int w, int h ) {
    /* Set draw colour to white */
    SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    
    /* clear the screen */
    SDL_RenderClear(g_renderer);
    
    SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

    int i;
    SDL_Rect cell = { 0, 0, CELL_SIZE, CELL_SIZE };
    for( i=0; i<w*h; i++ ) {
    	if( maze[i] & CELL_IN_MAZE ) {
    		cell.x = (i%w)*(CELL_SIZE << 1) + CELL_SIZE; 
    		cell.y = (i/w)*(CELL_SIZE << 1) + CELL_SIZE;
    		SDL_RenderFillRect(g_renderer, &cell);

    		if( !(maze[i]&WALL_EAST)) {
    			SDL_Rect east = { cell.x+CELL_SIZE, cell.y, cell.w, cell.h };
    			SDL_RenderFillRect(g_renderer, &east);
    		}

    		if( !(maze[i]&WALL_SOUTH) ) {
    			SDL_Rect south = { cell.x, cell.y+CELL_SIZE, cell.w, cell.h };
    			SDL_RenderFillRect(g_renderer, &south);
    		}
    	}    	
    }
    
    SDL_RenderPresent(g_renderer);
}

void
reset( void ) {
	g_generator[g_igen].init(&g_maze);
	SDL_SetWindowTitle(g_window, g_generator[g_igen].name);

}

void 
handle_events ( void ) {
    SDL_Event event;
    
    g_step = 1;
    while (SDL_PollEvent(&event)) {
    	switch(event.type) {
    	case SDL_QUIT:
    		g_user_quit = 1;
    		break;
    	case SDL_KEYDOWN:
    		switch(event.key.keysym.sym) {
    			case SDLK_1: case SDLK_2: case SDLK_3: case SDLK_4:
    				g_igen = event.key.keysym.sym-'1';
    			case SDLK_r:
    				reset();
    				break;
    			case SDLK_SPACE:
                    /* skip animation of maze generation */
    				g_step=-1;
    				break;
    			default:
    				break;
    		}
    	default:
    		break;
    	}
    }
}

void 
quit_display ( void ) {        
    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);
    SDL_Quit();
}

int
main( int argc, char *argv[] ) {	
	init_display();

	srand(time(NULL));
	maze_init( &g_maze, MAZE_WIDTH, MAZE_HEIGHT );
	reset();

	while(!g_user_quit) {				
		handle_events();
		g_generator[g_igen].step(&g_maze,g_step);
		render( g_maze.cells, g_maze.w, g_maze.h );
	}

	quit_display();

	return EXIT_SUCCESS;
}