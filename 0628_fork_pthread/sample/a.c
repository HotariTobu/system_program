#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <ctype.h>

int main(){
    int ch;
    while( (ch=getchar())!=EOF ) {
        if( isalpha(ch) ) { fprintf(stdout,"%c",ch); }
        else { fprintf(stderr,"%c",ch); }
    }
}
