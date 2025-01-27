(defun FSA (S)
	(setq fp (open S :direction :input)) ; read the file
	(state0 (read fp "done"))
	)

(defun state0(L)
(if (NULL L) (princ "Non-accepting State")) 
(setq x (CAR L))
(if (EQUAL 'x X) (state0 (CDR L)))
(if (EQUAL 'y X) (state1 (CDR L)))
)

(defun state1(L)
(if (NULL L) (princ "accepting State")) 
(setq x (CAR L))
(if (EQUAL 'x X) (state2 (CDR L)))
)

(defun state2(L)
(if (NULL L) (princ "Non-accepting State")) 
(setq x (CAR L))
(if (EQUAL 'x X) (state2 (CDR L)))
(if (EQUAL 'y X) (state3 (CDR L)))
)

(defun state3(L)
(if (NULL L) (princ "Non-accepting State")) 
(setq x (CAR L))
(if (EQUAL 'x X) (state3 (CDR L)))
(if (EQUAL 'z X) (state4 (CDR L)))
)

(defun state4(L)
(if (NULL L) (princ "Non-accepting State")) 
(setq x (CAR L))
(if (EQUAL 'x X) (state4 (CDR L)))
(if (EQUAL 'a X) (state1 (CDR L)))
)

