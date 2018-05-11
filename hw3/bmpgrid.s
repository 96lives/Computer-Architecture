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
    .LoopH:
        # rax=gap * bitWidth, rbx=wCounter, rcx=gap, rdx=height, rsi=first address of h, rdi=imgPtr
        # stack: [%rbx, width, gap, bitwidth
        movq $0, %rbx
        .LoopW:
            leaq (%rbx, %rbx, 2), %rcx
            movq $0, (%rsi, %rcx)
            movq $0, 1(%rsi, %rcx)
            movq $255, 2(%rsi, %rcx)
            addq $1, %rbx
            cmpq %rbx, 16(%rsp)
            jg .LoopW
        subq %rax, %rsi
        cmpq %rsi, %rdi
        jl .LoopH
#--------------draw Vertical Lines------------------------------------
    # rax=gap * bitWidth, rbx=pad, rcx=gap, rdx=height, rsi=?, rdi=imgPtr
    # stack: [%rbx, width, gap,  bitwidth

    popq %rax
    popq %rax
    popq %rax
    movq %rdi, %rax
    popq %rbx
    ret
