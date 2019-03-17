: prime 
 dup 2 < if 0 else 
 dup 2 = if 1 else 
 dup parity if 0 else 
 1 >r 
 repeat 
  dup
  r> 2 + dup >r 
 % 0 = until 
 r> = 
 then 
 then 
 then 
;
 

: test-allot
 prime
 1 allot 
 dup rot swap 
 ! 
;
