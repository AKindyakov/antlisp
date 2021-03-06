(defun fib_rec (target_n n prev_prev_sum prev_sum)
  (cond
    (
      (< n target_n)
      (fib_rec
        target_n (+ n 1) prev_sum (+ prev_prev_sum prev_sum)
      )
    )
    (t (+ prev_prev_sum prev_sum))
  )
)
(defun fib (n)
  (fib_rec n 0 0 1)
)
(load io)
(io:print
  "fib(10) -> " (fib 120)
)
