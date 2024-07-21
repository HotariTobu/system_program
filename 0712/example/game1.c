#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <sys/select.h>

void putspace(int k){ for( ; k>0 ; k-- ) putchar('.'); }

int main(){
    int k=30,ch='A',retval;
    fd_set rfds;
    struct timeval to;

    while(1){
        FD_ZERO(&rfds);
        FD_SET(0,&rfds);
        to.tv_sec=0;
        to.tv_usec=100000; // 100 * 1000 micro sec
        retval = select(1,&rfds,NULL,NULL,&to);
        if(retval == -1)perror("select()");
        else if(retval) { ch=getchar(); }

        putspace(k); printf("# ch=%c\n",ch);
        if( ch=='a') k--;
        else if( ch=='s' ) k++;
        ch=' ';
    }
}
