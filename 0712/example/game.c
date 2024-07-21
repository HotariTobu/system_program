#include <stdio.h>			/* 2024,7/12 kk@numinfo.ise.chuo-u.ac.jp */

void putspace(int k){ for( ; k>0 ; k-- ) putchar('.'); }

void nothing(){ int i,j,k;
    for(i=0;i<10000;i++) for(j=0;j<10000;j++);
}

int main(){
    int k=30,ch;
    while(1){
        putspace(k); printf("#\n");
        ch=getchar();
        if( ch=='a') k--;
        else if( ch=='s' ) k++;
        nothing();
    }
}
