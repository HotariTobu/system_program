#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <ncurses.h>

int main(){int i,ch;
    initscr();
    mvaddch(15,10,'A');
    move(0,0); addch('$');
    refresh();
}
