#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

int x,y,vx,vy,w,h;

void update(){
        x=x+vx; y=y+vy;
        if(x<1||x>w) vx=-vx;
        if(y<1||y>h) vy=-vy;
}

int main(int ac,char**av){int i,ch,seed=1;
        if(ac>1) seed=atoi(av[1]);
        srand(seed);
        w=70;h=20;
        initscr();
        x=rand()%w; y=rand()%h;
        vx=(rand()%2)*2-1; vy=(rand()%2)*2-1;

        for(i=0;i<1000;i++) {
                update();
                mvaddch(y,x,'#');
                //move(0,0); addch('$');
                refresh();
                usleep(100*1000);
        }
}
