(defun p_apply(fn args a)
; (print (list 'p_apply fn args a))
  (cond
   ((atom fn)
    (cond
     ((eq fn (quote car)) (caar args))
     ((eq fn (quote cdr)) (cdar args))
     ((eq fn (quote cons)) (cons (car args) (cadr args)))
     ((eq fn (quote atom)) (atom (car args)))
     ((eq fn (quote eq)) (eq (car args) (cadr args)))
     (t (p_apply (p_eval fn a) args a))
     ))
   ((eq (car fn) (quote lambda))
;   (print 'lambda)
    (p_eval (caddr fn) (p_pairlis (cadr fn) args a)))
   ((eq (car fn) (quote label))
;   (print 'label)
    (p_apply (caddr fn) args (cons (cons (cadr fn) (caddr fn)) a)))
   ))

(defun p_eval( e a)
; (print (list 'p_eval e a))
  (cond
   ((atom e) (cdr (p_assoc e a)))
   ((eq (car e) (quote quote)) (cadr e))
   ((eq (car e) (quote cond)) (p_evcon (cdr e) a))
   (t (p_apply (car e) (p_evlis (cdr e) a) a))
   ))

(defun p_evcon (c a)
;  (print (list 'p_evcon c a))
  (cond
   ((atom c) nil)
   ((p_eval (caar c) a) (p_eval (cadar c) a))
   (t (p_evcon (cdr c) a))
   ))

(defun p_evlis(m a)
;  (print (list 'p_evlis m a))
  (cond
   ((atom m) nil)
   (t (cons (p_eval (car m) a) (p_evlis (cdr m) a)))
   ))

(defun p_assoc ( x a)
  (cond
   ((atom a) nil)
   ((eq x (caar a)) (car a))
   (t (p_assoc x (cdr a)))
   ))

(defun p_pairlis (v e a)
  (cond
    ((atom v) a)
    (t (cons (cons (car v) (car e)) (p_pairlis (cdr v) (cdr e) a)))))

