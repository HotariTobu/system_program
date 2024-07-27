/* Usage: messerv [msgfile messerv.txt:default] [portno 58089:default] */
#include    <stdio.h>          /* 2024,7/5 kk@numinfo.ise.chuo-u.ac.jp */
#include    <stdlib.h>
#include    <unistd.h>
#include    <sys/wait.h>
#include    <netinet/in.h>
#include    <signal.h>
#include    <pthread.h>
#include    <string.h>
#include    <fcntl.h>
#include    <sys/select.h>

unsigned long Internet_Port=58089;

pthread_mutex_t mutex0=PTHREAD_MUTEX_INITIALIZER;

#define BUFMAX 20000
char recv_buf[BUFMAX];
char send_buf[BUFMAX];

struct sockaddr_in serv;
struct sockaddr_in from;
int s;  /* socket */
int g;  /* socket */

char *filename="messerv.txt";
char *default_message =
    "HTTP/1.0 200 Document follows\n"
    "Server: messerv\n"
    "Content-Type: text/html; charset=UTF-8\n"
    "\n"
    "No Proxy now\n";
char *not_found_message =
    "HTTP/1.1 404 Not Found\n"
    "Server: messerv\n"
    "Content-Type: text/html; charset=UTF-8\n"
    "\n"
    "404: Not Found\n";

#define MAX_PATH_LENGTH 1024
char path[MAX_PATH_LENGTH + 1];

char *replace_target = "MESSAGE";
char *get_replacement() {
    time_t t = time(NULL);
    return asctime(localtime(&t));
}

/* signal handler */
void catch_int() {
    fprintf(stderr,"End by INT\n");
    shutdown(g,SHUT_RDWR); shutdown(s,SHUT_RDWR); exit(0);
}

char *get_path(char *request_line) {
    int len;
    char *substr1, *substr2;

    substr1 = strstr(request_line, " /");
    if (substr1 == NULL) {
        return NULL;
    }

    substr1 += 2;

    substr2 = strstr(substr1, " ");
    if (substr2 == NULL) {
        return NULL;
    }

    len = substr2 - substr1;
    if (MAX_PATH_LENGTH < len) {
        perror("Too long path");
        return NULL;
    }

    strncpy(path, substr1, len);
    path[len] = '\0';

    return path;
}

void service(int client_fd) {
    int len;
    char *substr, *path = NULL;
    FILE *response_fp, *client_fp;

    len = read(client_fd, recv_buf, BUFMAX - 2);
    if (len == -1) {
        perror("read");
        return;
    }

    printf(recv_buf);
    path = get_path(recv_buf);

    client_fp = fdopen(client_fd, "w");
    if (client_fp == NULL) {
        perror("fdopen");
        return;
    }

    if (path == NULL || strlen(path) == 0) {
        response_fp = fopen(filename, "r");
        if (response_fp == NULL) {
            fprintf(client_fp, default_message);
        }
        else {
            while(fgets(send_buf, BUFMAX, response_fp) != NULL) {
                substr = strstr(send_buf, replace_target);
                if (substr == NULL) {
                    fprintf(client_fp, send_buf);
                }
                else {
                    substr[0] = '\0';
                    fprintf(client_fp, send_buf);
                    fprintf(client_fp, get_replacement());
                    fprintf(client_fp, substr + strlen(replace_target));
                }
            }

            fclose(response_fp);
        }
    }
    else {
        response_fp = fopen(path, "r");
        if (response_fp == NULL) {
            fprintf(client_fp, not_found_message);
        }
        else {
            while(fgets(send_buf, BUFMAX, response_fp) != NULL) {
                fprintf(client_fp, send_buf);
            }

            fclose(response_fp);
        }
    }

    fclose(client_fp);
}

void *callback(void *arg) {
    pthread_mutex_lock(&mutex0);
    service(*(int*)arg);
    close(g);
    pthread_mutex_unlock(&mutex0);
}

int main(int ac, char * av[]) {
    int ss, status, on=1;
    int pid;
    pthread_t t;

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
    if (listen(s, 5) != 0) {
        fprintf(stderr,"Failed to start listening...\n");
        exit(1);
    }
    while(1) {
        socklen_t len = sizeof(from);
        if( (g = accept(s, (struct sockaddr*)&from, &len)) < 0 ) {
            perror("accept"); exit(0);
        }

        if (pthread_create(&t, NULL, callback, &g) != 0) {
            fprintf(stderr,"Failed to create a thread...\n");
        }
    }
    return 0;
}
