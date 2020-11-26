#include "JohnB.h"

int main(){
	
	st_e_p h = NULL;	
	initscr();
	timeout(1);
        curs_set(0);
        cbreak();
        noecho();

  	WINDOW * win = newwin(ROWS, COLS, 0, 0);

	int _ccp = 0;
	player p(win, 3, 2, 'x', &h, &_ccp);
	int choice = ' ';
	while(choice != '0'){
		frame_refresh(win, &p, h, _ccp );
		choice = getch();
		p.kb_mv(choice);
		flushinp();
		
	}
	endwin();
	return 0;


}

