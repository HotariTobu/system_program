/* ポート番号50000を使用する. */ /* 2024,7/5 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define NSTRS   3         /* 文字列の個数 */

/* サーバに送る文字列 */
char *strs[NSTRS] = {
    "This is the first string from the client.\n",
    "This is the second string from the client.\n",
    "This is the third string from the client.\n"
};

int main(int ac, char *av[]){
    char c;
    FILE *fp;
    char hostname[64]="localhost";
    register int i, s;
    struct hostent *hp;
    struct sockaddr_in sin;

    /* まずホストネームを取得.    */
    if(ac>1) strcpy(hostname,av[1]); //gethostname(hostname, sizeof(hostname));

    /* つぎに, ホストのネットワークアドレスを取得.  */
    if((hp=gethostbyname(hostname))==NULL) {
        fprintf(stderr, "%s: unknown host.\n", hostname);
        exit(1);
    }

    /* 通信用ソケットを取得. このソケットはインターネット
     * ドメインで, ストリーム型(接続型)である.    */
    if((s=socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("client: socket");
        exit(1);
    }

    /* 接続先のネットワークアドレス情報を作成する.
     * ここではポート番号として, 50000を使用する.
     * 整数値はネットワークバイトオーダーに直すことに注意.
     * データは bcopy を用いて, 構造体に格納する.    */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(50000);
    bcopy(hp->h_addr_list[0], &sin.sin_addr, hp->h_length);

    printf("hostname=%s\n",hp->h_name);

    /* 上記アドレスに接続を試みる. この接続が成功するためには,
     * それまでにサーバー側で上記アドレスを取得し, listen()
     * システムコールを呼び出していなければならない.     */
    if(connect(s, (struct sockaddr*)&sin, sizeof(sin)) <0) {
        perror("client: connect");
        exit(1);
    }

    /* ソケットからデータを読み出すためのファイルポインタ */
    fp = fdopen(s, "r");

    /* 最初にサーバーから文字列を読み取り, それをプリントする. */
    for(i=0; i<NSTRS; i++) {
        while((c=fgetc(fp)) !=EOF ) {
            putchar(c);
            if(c=='\n')
                break;
        }
    }

    /* 今度はこちらから文字列をサーバーに送る.       */
    for(i=0; i<NSTRS; i++)
        send(s, strs[i], strlen(strs[i]), 0);

    /* ここで通信を切断するために, close() を用いる. */
    close(s);

    exit(0);
}
