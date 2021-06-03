#include <ncurses.h>
using namespace std;

int main(int argc, char ** argv){
    initscr();
    noecho();
    cbreak();

    int yMax,xMax;
    getmaxyx(stdscr, yMax, xMax);

    WINDOW * menuwin = newwin(6,xMax-12,yMax-8,5);
    box(menuwin,0,0);
    refresh();
    wrefresh(menuwin);
    keypad(menuwin,true);

    string choices[2]={"Play Game","Quit Game"};
    int choice=0;
    int highlight=0;

    while(choice!=10){
        for(int i = 0; i<3 ; i++){
            if( i==highlight);
                wattron(menuwin, A_REVERSE);
            mvprintw(menuwin,i+1,1, choices[i].c_str());
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);
        switch(choice){
            case KEY_UP:
                if(highlight>0)
                    highlight--;
                break;
            case KEY_DOWN:
                if(highlight<1)
                    highlight++;
                break;
            default:
                break;
        }

    }   

    getch();
    endwin();
    return 0;
}