# システムプログラム 第1回レポート

`print_expr`、`read_expr`で`quote_pointer`を使用するため、大域ポインタ類の宣言を`pname`より上部に移動させた。

## 出力中にQUOTE形式がある場合には引用符に変換して出力する

`print_expr`では渡された`Ptr`がシンボルではなかった場合、かつQUOTE形式だった場合、引用符を出力してから丸括弧を出力せずに後続の要素を出力するようにした。

書き換えたものが以下である。

```c
Ptr print_expr(Ptr x){
  if( symbolp(x) == true_pointer ) printf("%s",pname(x));
  else {
    if (car(x) == quote_pointer) {
      x=cdr(x);
      printf("'");
      print_expr(car(x));
      for(x=cdr(x); symbolp(x) == nil_pointer; x=cdr(x)) {
        printf(" ");
        print_expr(car(x));
      }
    }
    else {
      printf("(");
      print_expr(car(x));
      for(x=cdr(x); symbolp(x) == nil_pointer; x=cdr(x)) {
        printf(" ");
        print_expr(car(x));
      }
      if( x==nil_pointer ) printf(")");
      else printf(" . %s)", pname(x));
    }
  }
  return nil_pointer;
}
```

実行結果は以下のようになった。

```lisp
lisp> (quote a)
a
lisp> (quote (quote a))
'a
lisp> (quote (a b c))
(a b c)
lisp> (quote (quote (a b c)))
'(a b c)
lisp> (cons (quote a) (quote b))
(a . b)
lisp> (cons (quote (quote a)) (quote b))
('a . b)
lisp> (cons (quote (quote a)) (quote (quote b)))
('a quote b)
lisp>
```

## 入力中に引用符に続く1個の式がある場合にはQUOTE形式に変換して読み込む

`AtomName`に続けて`QuoteFormat`という値を追加した。
`next_token`では読み込んだ文字が引用符だった場合は`QuoteFormat`を返すようにした。
`read_expr_body`ではトークンが`QuoteFormat`だった場合は`quote_pointer`とその後に続く式を表すポインタを`cons`で繋げて返すようにした。

書き換えたものが以下である。

```c
enum{ AtomName=0x101, QuoteFormat=0x102 };

int next_token(){ /* 1文字読む. アトムのときは名前を読む. */
  int ch,i=0;
  while(isspace(ch=mygetchar())); /* スペースは読み飛ばす */
  if( ch==EOF ) { printf("exit\n"); exit(0); }
  if( ch=='\'' ) return QuoteFormat;
  if(isLispLetter(ch) ) {
    for(buf[i++]=ch; isLispLetter(ch=peekchar())&&i<MaxLength; buf[i++]=mygetchar());
    buf[i]=0;
    return AtomName;
  }
  return ch;
}

/*** 読み込み read_expr ***/
Ptr read_expr_body(int tk){
  if( tk==AtomName ) return intern(buf);
  else if( tk==QuoteFormat ) return cons(quote_pointer, cons(read_expr(), nil_pointer));
  else if( tk=='(' ) return read_expr_list(0);
  else { fprintf(stderr,"Format error(3): tk=%d\n", tk); exit(1); }
  return nil_pointer;
}
```

実行結果は以下のようになった。

```lisp
lisp> 'a
a
lisp> '(a b c)
(a b c)
lisp> (cons (quote quote) (quote(abc)))
'abc
```

<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/styles/default.min.css">
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/highlight.min.js"></script>

<!-- and it's easy to individually load additional languages -->
<script src="https://cdnjs.cloudflare.com/ajax/libs/highlight.js/11.9.0/languages/go.min.js"></script>

<script>hljs.highlightAll();</script>
