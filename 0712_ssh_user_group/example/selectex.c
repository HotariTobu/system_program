#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */
#include <sys/select.h>

int main(){
    char ch;
    fd_set rfds;
    struct timeval time_out;
    int retval;

    FD_ZERO(&rfds);
    FD_SET(0,&rfds);

    time_out.tv_sec=5;
    time_out.tv_usec=0;

    retval = select(1,&rfds,NULL,NULL,&time_out);

    if(retval == -1)perror("select()");
    else if(retval) {
        printf("Data is available\n");
        ch=getchar();
        printf(" ch=%c\n", ch);
    }
    else printf("No data\n");
}
