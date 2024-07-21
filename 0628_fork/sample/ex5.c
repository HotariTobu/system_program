#include <stdio.h>       /* 2024.06.28, kk@numinfo.ise.chuo-u.ac.jp */
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
int (*pp)[2];
void closeall(int n) { int i;
   for(i=0;i<n;i++) { close(pp[i][0]); close(pp[i][1]); }
}

int main(int ac, char *av[]) { int i,n=ac-1,ch,status; int pid[n];
    pp=malloc(sizeof(int[2])*n);
    for(i=0;i<n;i++) pipe(pp[i]);
    for(i=0;i<n;i++) {
        if((pid[i]=fork())==0) {
            close(0); dup(pp[i][0]); close(pp[i][0]);
            if(i<n-1) { close(1); dup(pp[i+1][1]); close(pp[i+1][1]); }
            closeall(n);
            system(av[i+1]);
            exit(0);
        } else if(pid[i]<0){ perror("fork() failed\n"); exit(1); }
    }
    close(1); dup(pp[0][1]); closeall(n);
    while((ch=getchar())!=EOF){putchar(ch);} fclose(stdout);
    { for(int i=0;i<n;i++) waitpid(pid[i],&status,0);} fprintf(stderr,"parent end\n");
    return 0;
}
