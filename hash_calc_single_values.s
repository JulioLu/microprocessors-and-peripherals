	.global hash_calc_single_values
    .text
	
hash_calc_single_values: 
	push    {lr}
	mov		r1 ,r0 // store the hash to th r1
	mov		r2, #0 // initialize count
	mov 	r4, #0 // initialize hash_value
	
loop:
	cmp 	r1, #0
	bne 	if_statement
	
	mov		r9, #10
	sdiv 	r5, r4, r9
	cmp 	r5, #0
	bne 	if_statement
	mov 	r0, r4
	
if_statement:
	cmp 	r1, #0
	beq 	if_statemant_for_zero_hash
	
	//calculate mod of hash->r1 with 10
	mov 	r9, #10
	sdiv    r6, r1, r9
    mul     r6, r6, r9 
    sub     r6, r1, r6 //r6->hash%10
	
	add 	r2, r2, r6
	sdiv    r1, r1, r9
	mov		r4, r2 
	
if_statemant_for_zero_hash:
	mov		r1, r2 
	mov		r2, #0
	
	
	
	
