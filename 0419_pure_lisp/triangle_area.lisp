(defun area
 (a b c)
 (
  let ((s (/ (+ a b c) 2)))
  (sqrt (* s (- s a) (- s b) (- s c)))
 )
)
