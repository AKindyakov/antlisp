(defun fibonacci(n)
  (cond
    ((= n 1) 0)
    ((= n 2) 1)
    (
        true (+ (fibonacci (+ n -1)) (fibonacci (+ n -2)))
    )
  )
)

(print (fibonacci 20))
