/* Usage: messerv [msgfile messerv.txt:default] [portno 58089:default] */
#include    <stdio.h>          /* 2024,7/5 kk@numinfo.ise.chuo-u.ac.jp */
#include    <stdlib.h>
#include    <unistd.h>
#include    <sys/wait.h>
#include    <netinet/in.h>
#include    <signal.h>

unsigned long Internet_Port=58089;

#define BUFMAX 20000
char recv_buf[BUFMAX];

struct sockaddr_in serv;
struct sockaddr_in from;
int s;  /* socket */
int g;  /* socket */

char* filename="messerv.txt";
char * default_message =
    "HTTP/1.0 200 Document follows\n"
    "Server: messerv\n"
    "Content-Type: text/html; charset=UTF-8\n"
    "\n"
    "No Proxy now\n";

/* signal handler */
void    catch_int() {
    fprintf(stderr,"End by INT\n");
    shutdown(g,SHUT_RDWR); shutdown(s,SHUT_RDWR); exit(0);
}
void service(int g) {
    int len,i,ch;
    FILE*fp=NULL,*fout;

    len=read(g,recv_buf,BUFMAX-2);
    if(len==-1) { perror("read"); close(g); exit(1); }

    for(i=0;i<len;i++) putchar(recv_buf[i]);
    fflush(stdout);

    //close(1); dup(g); close(g);
    fout=fdopen(g,"w");
    if(filename!=NULL && (fp=fopen(filename,"r"))!=NULL) {
        while( (ch=getc(fp))!=EOF) putc(ch,fout);
        fflush(fout);
    } else {
        fprintf(stdout,"%s\n", default_message);
    }
}
int main(int ac, char * av[]) {
    int ss, status, on=1;
    int pid;

    if(ac>1) filename = av[1];
    if(ac>2) Internet_Port = atoi(av[2]);

    s = socket(AF_INET, SOCK_STREAM, 0);
    printf("socket s=%d\n",s);
    if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,(char*)&on,sizeof(on))<0){
        perror("setsockopt");exit(1);}
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(Internet_Port);

    ss = bind(s, (struct sockaddr*)&serv, sizeof serv);
    printf("ss=%d\n", ss);
    if( ss < 0 ) {
        fprintf(stdout,"Can't bind\n");
        exit(1);
    }
    printf("Socket port #%d\n", ntohs(serv.sin_port));
    signal(SIGINT,catch_int);
    listen(s, 5);
    while(1) {
        socklen_t len = sizeof(from);
        if( (g = accept(s, (struct sockaddr*)&from, &len)) < 0 ) {
            perror("accept"); exit(0);
        }

        if ((pid=fork())==0) { service(g); exit(0); }

        wait(&status);
        close(g);
    }
    return 0;
}
