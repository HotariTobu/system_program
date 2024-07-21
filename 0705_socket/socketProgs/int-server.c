/* ポート番号50000を使用する. */ /* 2024,7/5 kk@numinfo.ise.chuo-u.ac.jp */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define NSTRS 3         /* 文字列の個数 */

/* クライアントに送る文字列 */
char *strs[NSTRS] = {
    "This is the first string from the server.\n",
    "This is the second string from the server.\n",
    "This is the third string from the server.\n"
};

int main() {
    char c;
    FILE *fp;
    register int i, s, ns;
    struct sockaddr_in sin, fsin;
    socklen_t fromlen;

    /* ソケットを取得. インターネットドメインで, ストリーム型  */
    if((s=socket(AF_INET, SOCK_STREAM, 0)) <0) {
        perror("server: socket");
        exit(1);
    }
    /* アドレス構造体を作成する. ここではポート番号 50000 を使用する.  */
    sin.sin_family = AF_INET;
    sin.sin_port = htons(50000);
    /* bcopy(hp->h_addr, &sin.sin_addr, hp->h_length); の代わりに, * 以下の INADDR_ANY を用いる.   */
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    /* アドレスとソケットを対応づける.   */
    if(bind(s, (struct sockaddr*)&sin, sizeof(sin))<0) {
        perror("server: bind");
        exit(1);
    }
    /* ソケットを通じて, サービス要求があるかどうか待機する.  */
    if(listen(s, 5)<0) {
        perror("server: listen");
        exit(1);
    }

    /* サービス要求があれば, それを受理して, 接続を確立する. 
     * 通進路を確立すると, ファイルディスクリプタ ns は
     * クライアント側と繋がったものになる. 
     * fsin にはクライアント側のアドレスが格納される.     */
    if((ns=accept(s, (struct sockaddr*)&fsin, &fromlen)) <0) {
        perror("server: accept");
        exit(1);
    }

    /* ソケット経由で, ファイルを読み出すファイルポインタ  */
    fp=fdopen(ns, "r");

    /* まず, 文字列をクライアントに送る.    */
    for(i=0; i<NSTRS; i++)
        send(ns, strs[i], strlen(strs[i]), 0);

    /* そして, クライアントから文字列を読み出す.  */
    for(i=0; i<NSTRS; i++) {
        while((c=fgetc(fp)) != EOF) {
        putchar(c);
        if(c=='\n')
            break;
        }
    }
    /* クライアントからの接続終了を待つ */
    fgetc(fp);

    /* ここで単に close() を呼び出して, 接続を切る.  */
    close(s);

    exit(0);
}
