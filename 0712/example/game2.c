#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <sys/select.h>
#include <termios.h>

struct termios tio;
int savedflag;

void putspace(int k){ for( ; k>0 ; k-- ) putchar('.'); }
void set_terminal_mode(){
        int rv=0, dummy;
        rv=tcgetattr(0, &tio); printf("rv=%d\n", rv);
        savedflag=tio.c_lflag;
        tio.c_lflag&=~ECHO;
        tio.c_lflag&=~ICANON;
        rv=tcsetattr(0, TCSANOW, &tio);
        printf("rv=%d\n", rv);
}
void reset_terminal_mode(){
        int rv=0, dummy;
        tio.c_lflag=savedflag;
        rv=tcsetattr(0, TCSANOW, &tio);
}
int main(){
    int i,k=30,ch='A', retval;
    fd_set rfds;
    struct timeval to;
    set_terminal_mode();
    while(1) {
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        to.tv_sec=0;
        to.tv_usec=100000; // 100 * 1000 micro sec
        retval = select(1,&rfds,NULL,NULL,&to);
        if(retval == -1)perror("select()");
        else if(retval) { ch=getchar(); }

        putspace(k); printf("# ch=%c\n",ch);
        if( ch=='a') { k--; } else if( ch=='s' ) { k++;}
        ch=' ';
    }
    //reset_terminal_mode();
}
