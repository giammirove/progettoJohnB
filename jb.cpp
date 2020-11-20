#include "jb.h"
/*struct elements{
	
	int cod;
	int x;
	int y;
	elements *next;
	
};

typedef elements *els;

bool nothing_in_the_way(els list, int yb, int xb)
{
	
		els tmp = list;

		while( tmp != NULL ){
			
			if( (tmp -> x == xb) && (tmp -> y == yb) ) return false;
			else tmp = tmp -> next;
		}
	return true;		
}
*/
bool kb_mv(WINDOW *win, int ch, int pch, int &y, int &x, int rows, int cols)
{
	if(ch >= 65 && ch <= 120){ 
		switch(ch){
			case 'a': case 'A':
				win_mv_l(win, pch, y, x, rows, cols);			  
			break;
	
			case 'D': case 'd':
				win_mv_r(win, pch, y, x, rows, cols);		
			break;

			case 'S': case 's':
				win_mv_d(win, pch, y, x, rows, cols);	
			break;

			case 'W': case 'w':
				win_mv_u(win, pch, y, x, rows, cols);	
			break;
		}
		return true;
	}
	else return false;
}

void win_mv_l(WINDOW *win, int ch, int &y, int &x, int rows, int cols)
{
	if( x > 0 ){
		mvwaddch(win, y, x, ' ');
		x--;
		mvwaddch(win, y, x, ch);
	}
}

void win_mv_r(WINDOW *win, int ch, int &y, int &x, int rows, int cols)
{
	
	if( x < cols ){
		mvwaddch(win, y, x, ' ');
		x++;
		mvwaddch(win, y, x, ch);
	}
}

void win_mv_u(WINDOW *win, int ch, int &y, int &x, int rows, int cols)
{
	
	if( y > 0 ){
		mvwaddch(win, y, x, ' ');
		y--;
		mvwaddch(win, y, x, ch);
	}
}

void win_mv_d(WINDOW *win, int ch, int &y, int &x, int rows, int cols)
{
	if( y < rows ){
		mvwaddch(win, y, x, ' ');
		y++;
		mvwaddch(win, y, x, ch);
	}
}

void win_mv_tp(WINDOW *win, int ch, int &y, int &x, int ny, int nx, int rows, int cols)
{
	if( ( ny > 0 && nx > 0 ) && ( ny < rows && nx < cols ) ){
		mvwaddch(win, y, x, ' ');
		y = ny;
		x = nx;
		mvwaddch(win, y, x, ch);
	}
}



void frame_refresh(WINDOW *win, int &refr_counter)
{
	usleep(tm);
	refr_counter ++;
	wborder(win, '*', '*', '*', '*', '*', '*', '*', '*' );
	wrefresh(win);
}

