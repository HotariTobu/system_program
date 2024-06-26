((lambda(app list4)
    ((label hanoi 
          (lambda(x y z l)
            (cond     
               ((atom (cdr l))
                (cons (list4 (quote move) x y (car l))nil))
               (t (app
                     (hanoi x z y (cdr l))
                     (cons (list4 (quote move) x y (car l))
                           (hanoi z y x (cdr l)))
                   )))))
     (quote a) (quote b) (quote c) (quote(1 2 3))
    ) 
 )
 (quote(lambda(x y)(cond((atom x)y)(t(cons(car x)(app (cdr x)y))))))
 (quote(lambda(x y z w)(cons x (cons y (cons z (cons w nil))))))
)
