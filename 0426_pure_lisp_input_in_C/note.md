# Lisp

S式(2分木の表現法の一つ)がプログラム、データ両方を表す
リストを処理する
ある特定の並びのリストを関数と引数とみなして実行
- リストを返す

6つの定義からインタプリタが作れる

## CAR

```lisp
(CAR (QUOTE (A B C)))
```

## CDR

```lisp
(CDR (QUOTE (A B C)))
```

## CONS

```lisp
(CONS (QUOTE A) (QUOTE B))
(CONS (QUOTE A) (QUOTE ()))
(CONS (QUOTE A) (QUOTE NIL))
(CONS (QUOTE A) ())
(CONS (QUOTE A) NIL)
NIL
```

## EQ

```lisp
(EQ (QUOTE A) (QUOTE B))
(EQ (QUOTE A) (QUOTE A))
(EQ (QUOTE (A B)) (QUOTE (A B)))
```

□□→□[NIL]
↓  ↓
A  B
↑  ↑
□□→□[NIL]

## ATOM

```lisp
(ATOM (QUOTE A))
(ATOM (QUOTE ABCDEF))
(ATOM (QUOTE A B C))
(ATOM (QUOTE (A B C)))
(ATOM NIL)
(ATOM (QUOTE ()))
```


## COND

条件分岐

```lisp
(COND
    (条件1 結果1)
    (条件2 結果2)
)
```

```lisp
(COND ((QUOTE A) (QUOTE B)) ((QUOTE C) (QUOTE D)))
(COND (() (QUOTE B)) ((QUOTE C) (QUOTE D)))
```

## LAMBDA

ラムダ関数

```lisp
(LAMBDA
    (引数リスト)
    本体
)
```

```lisp
(
    (LAMBDA
        (仮引数1 仮引数2 ...)
        本体
    )
    実引数1
    実引数2
    ...
)
```

```lisp
((LAMBDA (X Y) (CONS Y X)) (QUOTE A) (QUOTE B))
```

## APP

```lisp
((LABEL APP (LAMBDA (X Y) (COND ((ATOM X) Y) (t(CONS (CAR X) (APP (cdr X) Y))))) ))
```

(APP (A B) (C D))
↓
(A (APP (B) (C D)))
↓
(A (APP (B) (C D)))
↓
(A B (APP () (C D)))
↓
(A B (C D))
