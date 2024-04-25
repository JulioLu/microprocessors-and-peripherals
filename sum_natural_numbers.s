	.global sum_natural_numbers
    .text

sum_natural_numbers:
	push    {lr}
	mov 	r3, #0
L1:
	mov 	r1, r0 // store n to r1
	cmp 	r1, #0 // compare n with 0 
	bge 	topic_if_statement
	
	mov 	r0, r3 // return result 
	pop     {lr} 
	bx      lr
	
topic_if_statement:
	add		r3, r3, r1 // result = n
	sub 	r1, r1, #1 // n-1
	mov 	r0, r1 
	bl		L1

