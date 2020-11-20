#ifndef JB
#define JB

#include "jb.h"
#include <ncurses.h>
#include <unistd.h>

const int tm = 90000;
//PARAMETERS: (WINDOW, ch ( obtained with getch() ), player ASCII char, y, x, max rows, max cols)
bool kb_mv(WINDOW *win, int, int, int &, int &, int, int); //mv switch case

//PARAMETERS: (WINDOW, ch (entity's char), y, x, rows, cols) // used char
void win_mv_l(WINDOW *, int, int &, int &, int, int); //mv left
void win_mv_r(WINDOW *, int, int &, int &, int, int); //mv right
void win_mv_u(WINDOW *, int, int &, int &, int, int); //mv up
void win_mv_d(WINDOW *, int, int &, int &, int, int); //mv down

//PARAMETERS: (WINDOW, ch (entity's char), y, x, newy, newxm, rows, cols )
void win_mv_tp(WINDOW *, int, int &, int &, int, int, int, int);
//--------------------------------------------------------------------------------

void frame_refresh(WINDOW *, int &); 	//sleeps for tm microseconds 
				     	//and refreshes the WINDOW
					//. Game's clock


/*--------------------------------------------------------------------------------
ncurses functions to use:
	curs_set(0); //makes the cursor invisible
	timeout(1);  //sets getch() timer to 1 microseconds
	
	cbreak();
	noecho();
	wrefresh(WINDOW *);
	wborder(WINDOW *, char, char, char, char, char, char, char, char);
	keypad(WINDOW *, true); //enables keypad NOT SO USEFUL
	keypad(WINDOW *, false) //NOT SO USEFUL

	flushinp(); // flushes the getch() buffer
	
---------------------------------------------------------------------------------*/
#endif
