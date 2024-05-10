#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

enum
{
    LIST = 0,
    SYMBOL = 1
};
int dbg = 0;

typedef struct ptr
{
    int tag;
    union
    {
        // リストのための構造体
        struct
        {
            struct ptr *car;
            struct ptr *cdr;
        } cell;

        // シンボル(アトム)のための構造体
        struct
        {
            char *pname;
            struct ptr *plist;
        } symbol;
    } atr;
} *Ptr;

// 全てのアトムからなるリスト
Ptr oblist = NULL;

// シンボルTを指す
Ptr true_pointer;

// nilを指す
Ptr nil_pointer;

#pragma region 下請け関数
char *pname(Ptr x)
{
    return x->atr.symbol.pname;
}

Ptr symbolp(Ptr x)
{
    return (x->tag == SYMBOL) ? true_pointer : nil_pointer;
}

Ptr car(Ptr x)
{
    return x->atr.cell.car;
}

Ptr cdr(Ptr x)
{
    return x->atr.cell.cdr;
}
#pragma endregion

Ptr print_expr(Ptr x)
{
    if (symbolp(x) == true_pointer)
    {
        printf("%s", pname(x));
    }
    else
    {
        printf("(");
        print_expr(car(x));
        for (x = cdr(x); symbolp(x) == nil_pointer; x = cdr(x))
        {
            printf(" ");
            print_expr(car(x));
        }

        if (x == nil_pointer)
        {
            printf(")");
        }
        else
        {
            print(" . %s)", pname(x));
        }
    }

    return nil_pointer;
}
