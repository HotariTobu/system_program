#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>

int x,y,vx,vy,w,h; char**buf;

int main(int ac,char**av){int i,ch='s',seed=1,flag=1;
    if(ac>1) seed=atoi(av[1]);
    srand(seed);
    WINDOW*win=initscr();
    getmaxyx(win,h,w); // setup h and w
	buf=(char**)calloc(h,sizeof(char*));
	for(int i=0;i<h;i++)buf[i]=(char*)calloc(w,sizeof(char));
	for(int i=0;i<h;i++)for(int j=0;j<w;j++)buf[i][j]=' ';
    cbreak();
    noecho();
    nodelay(stdscr,TRUE);

    x=rand()%w; y=rand()%h; // initial position (y,x)
    vx=0; vy=0;

    for(i=0;i<1000;i++) {
        x+=vx; y+=vy;
        ch=getch();
        switch(ch){
            case 'a': case 'A': vx=-1; vy=0; break;
            case 's': case 'S': vx=1; vy=0; break;
            case 'z': case 'Z': vy=1; vx=0; break;
            case 'w': case 'W': vy=-1; vx=0; break;
            case ERR: if(flag>0){vx=1; vy=0;flag=0;} break;
        }
        if(x<0||y<0||x>=w||y>=h||buf[y][x]=='#') {
			move(y,(x<w-10?x:w-10)); addstr("conflict"); refresh();
			sleep(2); endwin(); 
			FILE*fp=fopen("z.output","w");
			for(int i=0;i<h;i++){
				for(int j=0;j<w;j++)fputc(buf[i][j],fp);
				fputc('\n',fp);
			}
			fclose(fp);
			exit(0);
		}
        mvaddch(y,x,'#'); buf[y][x]='#';   //move(0,0); addch('$');
        refresh();
        usleep(100*1000);
    }
}
