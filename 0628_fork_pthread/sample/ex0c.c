#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int i,pid,status;
    if( (pid=fork())==0 ) {
        printf("Child Process\n");
        for(i=0;i<10;i++) { printf("@\n"); fflush(stdout); sleep(1); }
    } else if( pid>0 ) {
        printf("Parent Process pid=%d\n",pid);
        for(i=0;i<5;i++) { printf("#\n"); fflush(stdout); sleep(1); }
        //kill(pid, SIGINT);
        waitpid(pid, &status, 0);
        printf("Parent end\n");
    } else { perror("fork() failed\n"); exit(1); }
    return 0;
}
