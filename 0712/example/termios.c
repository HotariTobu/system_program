#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <unistd.h>
#include <termios.h>

int main(){
    int rv=0, dummy, savedflag, restore_sw=0;
    struct termios tio;

    rv=tcgetattr(0, &tio);
    printf("rv=%d\n", rv);
    
    printf("tio.c_lflag=%x   ECHO=%x\n", tio.c_lflag, ECHO);
    savedflag=tio.c_lflag;

    tio.c_lflag&=~ECHO;    /* stty -echo   */
    tio.c_lflag&=~ICANON;  /* stty -icanon */

    rv=tcsetattr(0, TCSANOW, &tio);
    printf("rv=%d\n", rv);

	if( restore_sw ) {
        dummy=getchar();
        printf("dummy=%d\n", dummy);

        tio.c_lflag=savedflag;
        rv=tcsetattr(0, TCSANOW, &tio);
        printf("rv=%d\n", rv);
	}
}
