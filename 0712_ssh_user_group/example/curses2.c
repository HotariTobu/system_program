#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int main(){int i,ch,x,y;
    initscr();
    for(i=0;i<100;i++) {
        ch=getchar();
        x=rand()%70; y=rand()%20;
        mvaddch(y,x,'#');
        move(0,0); addch('$');
        refresh();
    }
}
