	.global sum_natural_numbers
    .text

sum_natural_numbers:
    PUSH    {LR}
	mov		r9, #1
	
	cmp 	r9, #1
	beq 	L1	
back:
	mov	 	r1, r0 // store input n 
	mov 	r2, #0 // result
	cmp 	r1, #0 // compare n with 0 
	bge 	topic_if_statement
	
	mov 	r0, r2 // return result
	bx      lr
	pop     {PC} 
	
L1:
	mov 	r3, #0
	bl 		back
	
	
topic_if_statement:
	add 	r3, r3, r1 // result = n
	sub 	r1, r1, #1 // n-1
	mov 	r0, r1
	mov		r9, #0
	bl		sum_natural_numbers

	
end:
    POP     {PC} 

