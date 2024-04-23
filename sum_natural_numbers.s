	.global sum_natural_numbers
    .text

sum_natural_numbers:
	push    {lr}
	mov 	r3, #0
L1:
	//mov		r9, #1
	
	//cmp 	r9, #1
	//beq 	L1	
	//bl 		back
	

	//mov 	r3, #0
	//bl 		back
	
	//ldr 	r1, [sp, #4] // store input n 
	//mov	 	r1, r0 // store input n 
	//mov 	r2, #0 // result
	mov 	r1, r0
	cmp 	r1, #0 // compare n with 0 
	bge 	topic_if_statement
	
	mov 	r0, r3
	pop     {lr} 
	bx      lr
	
topic_if_statement:
	//add 	r3, r3, r1 // result = n
	add		r3, r3, r1
	sub 	r1, r1, #1 // n-1
	mov 	r0, r1
	//mov		r9, #0
	bl		L1

