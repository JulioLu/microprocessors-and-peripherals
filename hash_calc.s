	.global hash_calc
    .text
map_array: 	.word 10, 42, 12, 21, 7, 5, 67, 48, 69, 2, 36, 3, 19, 1, 14, 51, 71, 8, 26, 54, 75, 15, 6, 59, 13, 25

hash_calc: 
	push    {lr}
	mov     r1, r0	// move the address of the char array to the r1
	mov		r2, #0 // initialize i 
	mov 	r4, #0 // initialize hash
	ldr		r7, =map_array
	
loop:
	ldrb	r3, [r1,r2] //input[i]
	cmp 	r3, #0
	beq 	end_loop
	
	cmp 	r3, #65
	bge		check_if_smaller_than_90

second_check:
	cmp 	r3, #97
	bge		check_if_smaller_than_122
		
	cmp 	r3, #48
	bge		check_if_smaller_than_59
	add 	r2, r2, #1
	b 		loop
	
check_if_smaller_than_90:
	cmp 	r3, #90
	ble 	capital_hash
	b 		second_check
	
check_if_smaller_than_122:
	cmp 	r3, #122
	ble 	lower_hash
	add 	r2, r2, #1
	b 		loop
	
check_if_smaller_than_59:
	cmp 	r3, #59
	ble 	number_hash
	add 	r2, r2, #1
	b 		loop
	
capital_hash:
	sub		r5, r3, #65 //input[i]-65
	ldrb	r6, [r7,r5, LSL #2] //map_array[input[i]-65]
	add 	r4, r4, r6
	add 	r2, r2, #1
	b		loop

lower_hash:
	sub		r5, r3, #97 //input[i]-97
	ldrb	r6, [r7,r5, LSL #2] //map_array[input[i]-97]
	//ldrb	r6,[r7],
	sub 	r4, r4, r6
	add 	r2, r2, #1
	b     	loop
	
number_hash:
	sub		r5, r3, #59 //input[i]-59
	sub 	r6, r3, #48 // input[i] - 48
	add 	r4, r4, r6 
	add 	r2, r2, #1
	b		loop
	
end_loop:
	mov 	r0, r4
	pop 	{lr}
	bx 		lr
