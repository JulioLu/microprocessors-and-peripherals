	.global hash_calc_single_value
    .text
	
hash_calc_single_value: 
	push    {lr}
	mov		r1 ,r0 // store the hash to th r1
	mov		r2, #0 // initialize count -> supportive variable for the calculations 
	mov 	r4, #0 // initialize hash_value -> the single digit result 
	
loop:
	cmp 	r1, #0
	bne 	if_statement
	
	mov		r9, #10
	sdiv 	r5, r4, r9 // r5 = hash_value/10
	cmp 	r5, #0
	bne 	if_statement
	mov 	r0, r4 // return single digit result
	pop 	{lr}
	bx 		lr
	
if_statement:
	cmp 	r1, #0
	beq 	if_statemant_for_zero_hash
	
	//calculate mod of hash->r1 with 10
	mov 	r9, #10
	sdiv    r6, r1, r9
    mul     r6, r6, r9 //r6->hash%10
    sub     r6, r1, r6 // subtracting r6 from the input hash in since the last digit has been added to the sum.
	
	add 	r2, r2, r6 // add the last digit to the supportive register r2 
	sdiv    r1, r1, r9
	mov		r4, r2 // move the value of the supportive register r2 to the output register r4
	bl 		loop
	
if_statemant_for_zero_hash:
	mov		r1, r2 
	mov		r2, #0
	bl 		if_statement
	
	
	
	
