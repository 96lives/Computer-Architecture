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
    pushq %rcx
    pushq %rsi
    pushq %rdx
    leaq -1(%rdx), %rax
    imul %rsi
    xchg %rax, %rsi
    imul %rcx
    addq %rdi, %rsi
    popq %rdx
    # rax=gap * bitWidth, rbx=pad, rcx=gap, rdx=height, rsi=imgEnd-bitWidth, rdi=imgPtr
    # stackL [%rbx, width, gap,  bitWidth
    .LoopHH:
        # rax=gap * bitWidth, rbx=wCounter, rcx=gap, rdx=height, rsi=first address of h, rdi=imgPtr
        # stack: [%rbx, width, gap, bitwidth
        movq $0, %rbx
        .LoopHW:
            leaq (%rbx, %rbx, 2), %rcx
            movb $0, (%rsi, %rcx)
            movb $0, 1(%rsi, %rcx)
            movb $255, 2(%rsi, %rcx)
            incq %rbx
            cmpq %rbx, 16(%rsp)
            jg .LoopHW
        subq %rax, %rsi
        cmpq %rsi, %rdi
        jl .LoopHH
#--------------draw Vertical Lines------------------------------------
    # rax=gap * bitWidth, rbx=?, rcx=?, rdx=height, rsi=?, rdi=imgPtr
    # stack: [%rbx, width, gap,  bitwidth
    popq %rax
    movq %rax, %rsi
    mulq %rdx
    popq %rcx
    movq $0, %rdx
    addq %rdi, %rax
    pushq %rdi
    pushq %rax
    # rax=imgEnd, rbx=?, rcx=gap, rdx=wCounter, rsi=bitWidth, rdi=imgPtr
    # stack: [%rbx, width, imgPtr, imgEnd
    .LoopVH:
        movq $0, %rdx
        .LoopVV:
            leaq (%rdx, %rdx, 2), %rax
            movb $0, (%rdi, %rax)
            movb $0, 1(%rdi, %rax)
            movb $255, 2(%rdi, %rax)
            addq %rcx, %rdx
            cmpq %rdx, 16(%rsp)
            jg .LoopVV
        addq %rsi, %rdi
        cmpq %rdi, (%rsp)
        jg .LoopVH
    popq %rax
    popq %rax
    popq %rbx
    popq %rbx
    ret
