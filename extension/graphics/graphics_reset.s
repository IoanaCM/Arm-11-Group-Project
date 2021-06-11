.text
graphics_reset:
	
	@ register parameters: 
	@ r0  <- x0
	@ r1  <- y0
	@ r2  <- width  (X axis)
	@ r3  <- height (Y axis)

	@ internal registers for local variables: 
	@ r5  <- x iterator
	@ r6  <- y iterator
	@ r8  <- X-axis target pixel index
	@ r9  <- Y-axis target pixel index
	@ r10 <- array iterator index
    @ r11 <- auxiliary register 
    @ r12 <- background register
    @ r15 <- write_image_buffer register
		

    push r5
	push r6
	push r8
	push r9
	push r10
	push r11
    push r12
    push r15

    @ y-axis iterator
	mov r6, r1
    @ x-axis iterator
	mov r5, r0

    @ set target width-x
	mov r8, r2
	add r8, r1

    @ set target height-y
	mov r9, r3 
	add r9, r9, r1

    @ array iterator
	mov r11, height
	mla r10, r0, r11, r1

    mov r12 #:first8:background
    orr r12 #:second8:background
    orr r12 #:third8:background
    orr r12 #:fourth8:background

    mov r15 #:first8:write_image_buffer
    orr r15 #:second8:write_image_buffer
    orr r15 #:third8:write_image_buffer
    orr r15 #:fourth8:write_image_buffer


	condX:
		cmp r5, r8
		bgt end

	condY:
		cmp r6, r9
		ble loop @ y-iterator <= target-y

		sub r6, r6, r3

		sub r10, r10, r3
		add r10, r10, height

		add r5, r5, #1
		b condX	

	loop:
        ldr r11, [r12, r10]
		str r11, [r15, r10] 
		add r6, r6, #1
		add r10, r10, #1
		b condY

    end:
        pop r15
        pop r12
		pop r11
		pop r10
		pop r9
		pop r8
		pop r6
		pop r5
		ret    