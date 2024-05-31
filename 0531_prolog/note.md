# Prolog

aが成立するにはb, cがyesである必要がある。

```prolog
a :- b, c
```

```prolog
app([1], [2], [1, 2]).
```

が実行されると

```prolog
app([A|X], Y, [A|Z]) :- app(X, Y, Z).
```

にマッチする。

```prolog
app([A|X], Y, [A|Z])
```

で

- A: 1
- X: nil
- Y: 2
- Z: 2

となり

```prolog
app(X, Y, Z)
```

にマッチする。

```prolog
app([], 2, 2)
```

## 述語論理で難しいこと

- 否定
  - 飛ばない鳥は？
