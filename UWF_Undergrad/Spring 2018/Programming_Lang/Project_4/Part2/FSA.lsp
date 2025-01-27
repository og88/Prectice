(defun demo ()
	(setq fp (open "commands.txt" :direction :input)) ; read the file
	(setq s (read fp))
	(state0 s)
	(setq s (read fp))
	(state0 s)
	(setq s (read fp))
	(state0 s)
	(setq s (read fp))
	(state0 s)
	(setq s (read fp))
	(state0 s)
	)

(defun state0(L) 
(setq x (CAR L))
(princ x)
(cond 
((EQUAL nil L)(princ "\nNon-accepting State\n" ))
((EQUAL 'x X) (state0 (CDR L)))
((EQUAL 'y X) (state1 (CDR L)))
(t (princ "\nInvalid\n"))
))

(defun state1(L) 
(setq x (CAR L))
(princ x)
(cond 
((EQUAL nil L)(princ "\naccepting State\n" ))
((EQUAL 'x X) (state2 (CDR L)))
(t (princ "\nInvalid\n"))
))

(defun state2(L) 
(setq x (CAR L))
(princ x)
(cond 
((EQUAL nil L)(princ "\nNon-accepting State\n" ))
((EQUAL 'x X) (state2 (CDR L)))
((EQUAL 'y X) (state3 (CDR L)))
(t (princ "\nInvalid\n"))
))

(defun state3(L) 
(setq x (CAR L))
(princ x)
(cond 
((EQUAL nil L)(princ "\nNon-accepting State\n" ))
((EQUAL 'x X) (state3 (CDR L)))
((EQUAL 'z X) (state4 (CDR L)))
(t (princ "\nInvalid\n"))
))

(defun state4(L) 
(setq x (CAR L))
(princ x)
(cond 
((EQUAL nil L)(princ "\nNon-accepting State\n" ))
((EQUAL 'x X) (state4 (CDR L)))
((EQUAL 'a X) (state1 (CDR L)))
(t (princ "\nInvalid\n"))
))

