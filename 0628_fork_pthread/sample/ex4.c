#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

char *comline1, *comline2;
int p1[2], p2[2];

void child1() { /* child1 */
   close(0); dup(p1[0]); close(p1[0]); close(p1[1]);
   close(1); dup(p2[1]); close(p2[0]); close(p2[1]);
   system(comline1); exit(0);
}
void child2() { /* child2 */
   close(0); dup(p2[0]); close(p2[0]); close(p2[1]); close(p1[0]); close(p1[1]);
   system(comline2); exit(0);
}
void parent(int pid1, int pid2) { int ch; /* parent */
   close(1); dup(p1[1]); close(p1[0]); close(p1[1]); close(p2[0]); close(p2[1]);
   while((ch=getchar())!=EOF){putchar(ch); fflush(stdout);} close(1); 
   sleep(1);
}
int main(int ac, char *av[]) { int pid1, pid2;
    if(ac==3) { comline1 = av[1]; comline2 = av[2]; }
    else { fprintf(stderr,"%s comline1 comline2\n", av[0]); exit(1); }
    pipe(p1); pipe(p2);
    if((pid1=fork())==0) { child1(); }
    else if(pid1>0 && (pid2=fork())==0) { child2(); }
    else if(pid1>0 && pid2>0) { parent(pid1,pid2); }
    else { perror("fork() failed\n"); exit(1); }
    return 0;
}
