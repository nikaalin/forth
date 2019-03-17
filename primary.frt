: primary 
	2 >r ( s: n r: b)
	repeat 
		dup r@ % 0 = if 
			r@ prime if
				dup r@ / r@ % 0 = if
					0 >r ( flag for end of cycle)
				else
					r@ / 
					r> 1 + >r
					1 >r
				then
			else
				0 >r
			then
		else
			r> 1 + >r
			1 >r
		then
	dup 1 > r> land not until
	r> drop 1 = 		
;
