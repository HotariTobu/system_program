((lambda(app)(app (quote(a b c)) (quote(d e f)) ))
 (quote(lambda(x y)(cond((atom x)y)(t(cons(car x)(app(cdr x)y)))))))
