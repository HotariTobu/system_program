#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>

int x=0;

int main(){
    int i,pid;
    if( (pid=fork())==0 ) { /* fork()の戻り値が 0 の場合：子プロセス */
        printf("Child Process pid=%d\n", pid);
        for(i=0;i<10;i++) { printf("@x=%d\n",x++); fflush(stdout); sleep(1); }
        printf("Child Process End\n");
    } else if( pid>0 ) {  /* fork()の戻り値が 正整数 の場合：親プロセス */
        printf("Parent Process pid=%d\n",pid);
        for(i=0;i<10;i++) { printf("#x=%d\n",x++); fflush(stdout); sleep(1); }
        printf("Parent Process End\n");
    } else { perror("fork() failed\n"); exit(1); }
    return 0;
}
