#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int p1[2], p2[2];
void child1() { /* child1 */
    close(0); dup(p1[0]); close(p1[0]); close(p1[1]);
    close(1); dup(p2[1]); close(p2[0]); close(p2[1]);
    execl("/usr/bin/tr", "/usr/bin/tr", "a-zA-Z", "A-Za-z", (void*)0);
    perror("can't execl"); 
    exit(1);
}
void child2() { /* child2 */
    close(0); dup(p2[0]); close(p2[0]); close(p2[1]);
    close(p1[0]); close(p1[1]);
    execl("/usr/bin/tr", "/usr/bin/tr", "A-Z", "B-ZA", (void*)0);
    perror("can't execl"); 
}
void parent(int pid1, int pid2) { int ch; /* parent */
    close(1); dup(p1[1]); close(p1[0]); close(p1[1]);
    close(p2[0]); close(p2[1]);
    while((ch=getchar())!=EOF) {putchar(ch);} fflush(stdout);
}
int main() { int pid1, pid2;
    pipe(p1); pipe(p2);
    if((pid1=fork())==0) { child1(); }
    else if(pid1>0 && (pid2=fork())==0) { child2(); }
    else if(pid1>0 && pid2>0) { parent(pid1,pid2); }
    else { perror("fork() failad\n"); }
    return 0;
}
