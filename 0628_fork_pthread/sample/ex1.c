#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
int  mark = 0;
char marks[]={'!','@','#','$','%'};
/***********************************************/
void child_signal_handler() {
    printf("\nchild: get signal\n");
    mark = (mark+1)%5;
}
void child() {
    fclose(stdin);
    signal(SIGINT, child_signal_handler);
    while(1) {
        sleep(1);
        printf("%c", marks[mark]); 
        fflush(stdout);
    }
}
/***********************************************/
void parent_signal_handler() {
    killpg(getpgrp(), SIGKILL);          /* 親プロセス終了の処理 */
}
void parent(int pid) { int ch, status;
    signal(SIGINT, parent_signal_handler); /* 親プロセス終了検知 */
    //fclose(stdout);
    while((ch=getchar())!=EOF) {
        printf("%c", ch); fflush(stdout); kill(pid,SIGINT); }
    printf("Waits end of child process");fflush(stdout);
    while(wait(&status)!=pid);
}
/***********************************************/
int main() { int pid;
    if((pid=fork())==0){ child(); } else if(pid>0){ parent(pid); } else{ exit(1); } return 0;
}
