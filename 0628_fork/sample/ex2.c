#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main() { int pid,ch; int p1[2];
    pipe(p1); /* pipe system call */
    if((pid=fork())==0){
        char*argv[]={"/usr/bin/tr","a-zA-Z","A-Za-z",(void*)0};
        close(0); dup(p1[0]); close(p1[0]); close(p1[1]);
        execve("/usr/bin/tr",argv,NULL);
        //execl("/usr/bin/tr", "/usr/bin/tr", "a-zA-Z", "A-Za-z", (void*)0);
        perror("can't execl"); 
        exit(1);
    } else if(pid>0) {
        /* parent */
        close(1); dup(p1[1]); close(p1[0]); close(p1[1]);
        while((ch=getchar())!=EOF) { putchar(ch); /* fflush(stdout);*/ }
        fclose(stdout); /* fflush(stdout); close(1); */
        sleep(1);
    } else { perror("fork() failed.\n"); exit(1); }
    return 0;
}
