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
# returns the current starting address
drawHorLine:
	pushq %rbx
	# %rbx is the counter that counts till width
	movq $0, %rbx
	.drawDotRed:
		movb $0, (%rax, %rbx)
		movb $0, 1(%rax, %rbx)
		movb $255, 2(%rax, %rbx)
		addq $3, %rbx
		cmpq %rbx, %rsi
		jg .drawDotRed
	popq %rbx
	ret

# Assumes %rax=current starting address, %rbx=imgPtr, %rcx=gap, %rdx=height, %rdi=imgEnd, %rsi=bitWidth
# Stack: []

drawVerLines:

    # %rbx is the counter
    pushq %rbx
    pushq %rcx
    leaq (%rcx, %rcx, 2), %rcx
    movq $0, %rbx
    .drawVer:
        movb $0, (%rax, %rbx)
        movb $0, 1(%rax, %rbx)
        movb $255, 2(%rax, %rbx)
        addq %rcx, %rbx
        cmpq %rbx, %rsi
        jg .drawVer
    popq %rcx
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
	pushq %rbx

	# make width = 3 * width + pad
	pushq %rsi
	movq %rsi, %rbx
	andq $3, %rbx
	leaq (%rsi, %rsi, 2), %rsi
    addq %rbx, %rsi

    # rax=?, rbx=pad, rcx=gap, rdx=height, rsi=bitWidth, rdi=imgPtr
    # stack=[%rbx, width

#---Draw Horizontally-------------------------------------
    leaq -1(%rdx), %rax
    imul %rsi
    pushq %rsi
    xchg %rax, %rsi

    # rax=imgEnd-bitWidth, rbx=pad, rcx=gap, rdx=height, rsi=bitWidth, rdi=imgPtr
    # stackL [%rbx, width, bitWidth
    .LoopH:
        # rax=imgEnd-bitWidth, rbx=wCounter, rcx=gap, rdx=height, rsi=bitWidth, rdi=imgPtr
        # stack: [%rbx, width, bitwidth
        movq $0, %rbx
        .LoopW:
            movq $0, (, %rbx, 3)
            movq $0, 1(, %rbx, 3)
            movq $255, 2(, %rbx, 3)
            addq $1, %rbx
            cmpq %rbx, 8(%rsp)
            jle .LoopW
        subq %rsi, %rax
        cmpq %rax, %rdi




	# move the pointer to the return value
	movq %rdi, %rbx

	# %rax keeps the address to start of the row
	# %rdi keeps the end of the img



	# Stack: []
	.drawVerLines:
	    call drawVerLines
	    addq %rsi, %rax
	    cmpq %rax, %rdi
	    jg .drawVerLines

	#------------Pad correction--------------

	popq %rdx # pop pad
    movq %rbx, %rax
	popq %rbx


	ret
