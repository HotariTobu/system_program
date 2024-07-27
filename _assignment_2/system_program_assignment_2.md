# システムプログラム 第2回レポート

## pthreadを使用する

pthreadの使用するために8行目でヘッダファイルをインクルードしている。

```c
#include    <pthread.h>
```

スレッドで実行する処理を142-147行目の関数`callback`で定義している。

```c
void *callback(void *arg) {
    pthread_mutex_lock(&mutex0);
    service(*(int*)arg);
    close(g);
    pthread_mutex_unlock(&mutex0);
}
```

排他制御を行うためにmutexを使用している。
引数としてソケットが渡されることを想定しており、引数からソケットを取得して関数`service`を呼び出している。

関数`main`の152行目でスレッドの変数の宣言を行い、183-185行目でスレッドの作成を行っている。

```c
pthread_t t;

if (pthread_create(&t, NULL, callback, &g) != 0) {
    fprintf(stderr,"Failed to create a thread...\n");
}
```

スレッドを待機すると、そのスレッドの処理が終了するまでサーバとして応答できなくなってしまう。
そのためスレッドの待機は行わず、ソケットの切断はスレッドの処理の最後に行っている。

## MESSAGEをアクセス日時に置き換える

文字列を処理するために9行目でヘッダファイルをインクルードしている。

```c
#include    <string.h>
```

置換対象の文字列`replace_target`と置換後の文字列を取得する関数`get_replacement`を43-47行目で定義している。

```c
char *replace_target = "MESSAGE";
char *get_replacement() {
    time_t t = time(NULL);
    return asctime(localtime(&t));
}
```

109-120行目でメッセージファイルの内容をクライアントに返している。

```c
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
```

文字列の検索を行うためにメッセージファイルは行単位で読み込む。
読み込んだ文字列に置換対象の文字列が含まれていれば、その部分を除外して代わりに置換後の文字列を挿入する。

## その他の拡張

### パスによって読み込むファイルを変える

HTTPリクエストのパスで指定されたファイルの内容をレスポンスで返す機能を追加した。

例えば

```http
GET /hoge.txt HTTP/1.1
```

というリクエストラインで始まるHTTPリクエスト受け取った場合、ワーキングディレクトリに`hoge.txt`が存在すれば、その内容をリクエストとして返す。

リクエストラインからパスを取得する関数`get_path`を55-81行目で定義している。

```c
char *get_path(char *request_line);
```

関数`service`では95行目でHTTPリクエストからパスを取得し、126-136行目でパスに応じたレスポンスを返す。
パスが指すファイルが存在しない場合は、ステータスコードが`404`のレスポンスを返す。

```c
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
```

以下は`curl`コマンドを使用したリクエストとレスポンスの例である。

```sh
# curl localhost:58089
<head>
<title> no proxy message </title>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<script>
<!---
th_color = "#fffeee";
td_color = "#aaffff";
table_border = 10;

function MakeArray(n) {
    this.length = n; for(var i=1;i<=n;i++) {this[i]="　";}
        return this;
}
h = new MakeArray(7);
g1 = new MakeArray(7);
g2 = new MakeArray(7);
g3 = new MakeArray(7);
g4 = new MakeArray(7);
g5 = new MakeArray(7);


// Data
h[1]="時限｜曜日"; h[2]="月"; h[3]="火"; h[4]="水"; h[5]="木";
h[6]="金"; h[7]="土";
g1[1]="第１時限 <br>(9:00〜10:40)";
g2[1]="第２時限 <br>(10:50〜12:30)";
g3[1]="第３時限 <br>(13:20〜15:00)";
g4[1]="第４時限 <br>(15:10〜16:50)";
g5[1]="第５時限 <br>(17:00〜18:40)";

//*********************************************************************
g2[2]="プログラミング上級演習<br>森口";
g4[2]="プログラミング上級演習<br>森口";
g2[3]="Cプログラミング初級演習<br>今堀";
g3[3]="Cプログラミング初級演習<br>今堀";
g2[6]="基盤系オブジェクト指向プログラミング演習<br>高松";
g3[6]="基盤系オブジェクト指向プログラミング演習<br>高松";
g3[4]="実践プログラミング<br>森口";

//*********************************************************************
// End Data

function print_a_row(h,border,th_color,td_color) {
    document.write("<tr><th bgcolor=",th_color," border=",
                table_border," width=5% >",h[1],"</th>");
    for(var i=2; i<=7; i++) {
        document.write("<td align=center bgcolor=",td_color," width=18%>",
                              h[i],"</td>");
    }
    document.write("</tr>");
    document.write("<br>");
}

function show_table() {
    document.write("<table border=",table_border,">");
    print_a_row(h,table_border,td_color,th_color);
    print_a_row(g1,table_border,th_color,td_color);
    print_a_row(g2,table_border,th_color,td_color);
    print_a_row(g3,table_border,th_color,td_color);
    print_a_row(g4,table_border,th_color,td_color);
    print_a_row(g5,table_border,th_color,td_color);
    document.write("</table>");
}
// -->
</script>
</head>
<body bgcolor=#eeeeff>
演習室時間割 （20xx年度前期）<br>
 Sun Jul 21 15:19:18 2024
  <br>
下記の時間は授業で使用します.
<script>
<!----
show_table();
// -->
</script>
</body>
```

```sh
# curl localhost:58089/messerv2.txt
<!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
</head>

<body>
    Content
</body>

</html>
```

```sh
# curl localhost:58089/hoge.txt
404: Not Found
```

## おわりに

スレッドの処理全体をmutexのロックで囲ってしまっているため、実際にはリクエストが同期的に処理されると考えられる。
リクエストを非同期的に処理するには、mutexのロックの範囲をリクエストの読み込み、パスの取得、レスポンスの書き込み、というように細かく分ける必要があると考えられる。

リクエストを行単位で読み込むために`fgets`を使用しようとしたが、すべてのリクエストの内容を読み込んだ後に`fgets`が処理をブロックしてしまい、うまくいかなかった。
`fgets`が処理をブロックする前に読み込みを終了するために`select`を使用しようとしたが、すべてのリクエストの内容を読み込む前に読み込みが終了してしまい、うまくいかなかった。
以下はその時の関数`service`の一部である。

```c
flags = fcntl(client_fd, F_GETFL);
fcntl(client_fd, F_SETFL, flags | O_NONBLOCK);

client_fp = fdopen(client_fd, "w+");
if (client_fp == NULL) {
    perror("fdopen");
    return;
}

if (fgets(recv_buf, BUFMAX, client_fp) != NULL) {
    printf(recv_buf);
    path = get_path(recv_buf);
}

while (1) {
    FD_ZERO(&rfds);
    FD_SET(client_fd, &rfds);

    time_out.tv_sec = 0;
    time_out.tv_usec = 10000;

    retval = select(client_fd + 1, &rfds, NULL, NULL, &time_out);
    if (retval < 0) {
        perror("select");
        return;
    }

    if (retval == 0) {
        break;
    }

    if (fgets(recv_buf, BUFMAX, client_fp) != NULL) {
        printf(recv_buf);
    }
}
```
