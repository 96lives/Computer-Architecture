#---------------------------------------------------------
# 
#  Project #3: Drawing grid lines in an image
#
#  April 30, 2018
#
#  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#
#---------------------------------------------------------


.text
	.align 4
.globl bmp_grid
	.type bmp_grid,@function


# %rbx always has the imgptr



#------------------------------------------------------
# %rax: current starting address, %rsi: bitWidth
# returns the next starting address
drawHorLine:
	
	pushq %rbx
	# %rbx is the counter that counts till width
	movq $0, %rbx
	.drawDotRed:
		movq $0, (%rax, %rbx)
		movq $0, 1(%rax, %rbx)
		movq $255, 2(%rax, %rbx)
		addq $3, %rbx
		cmpq %rbx, %rsi
		jg .drawDotRed
	cmpq %rbx, %rsi
	je .done
	subq $1, %rbx
	cmpq %rbx, %rsi
	je .done
	subq $1, %rbx

	.done:
		leaq (%rax, %rbx), %rax
		popq %rbx
		ret


bmp_grid:
	#------------------------------------------------------------
	# Use %rax, %rbx, %rcx, %rdx, %rsi, and %rdi registers only
	#	imgptr is in %rdi
	#	width  is in %rsi
	#	height is in %rdx
	#   gap	   is in %rcx
	#-----------------------------------------------------------

	# make width = 3 * width + pad
	
	leaq (%rsi, %rsi, 2), %rsi
	movq %rsi, %rbx
	andq $3, %rbx
	cmpq $0, %rbx
	jne .notZero
	movq $4, %rbx
	.notZero:
	negq %rbx
	addq $4, %rbx
	addq %rbx, %rsi

	# gap = gap + 1
	leaq 1(%rcx), %rcx
	
	# move the pointer to the return value
	movq %rdi, %rbx

#---------------------------------------------------------
	# draw rows
	# %rax keeps the address to start of the row
	# %rdi keeps the end of the img
	
	movq %rsi, %rax
	pushq %rdx
	mulq %rdx
	popq %rdx
	addq %rax, %rdi

	# Now: %rax=?, %rbx=imgPtr, %rcx=gap, %rdx=height, %rdi=imgEnd, %rsi=bitWidth
	# Stack: []
    pushq %rcx
    pushq %rdx
    movq %rcx, %rax
    mulq %rsi
    movq %rax, %rcx

    movq %rdi, %rax
    subq %rsi, %rax
    popq %rdx

 	# Now: %rax=imgEnd-bitWidth, %rbx=imgPtr, %rcx=gap*bitWidth, %rdx=height, %rdi=imgEnd, %rsi=bitWidth
	# Stack: [gap]

    .drawHorLines:
		call drawHorLine
		subq %rcx, %rax
		cmpq %rbx, %rax
		jge .drawHorLines

	popq %rcx
	
	movq %rbx, %rax

	#------------------------------------------------------------

	ret
