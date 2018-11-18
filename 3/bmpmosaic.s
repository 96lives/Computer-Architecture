#--------------------------------------------------------------
# 
#  4190.308 Computer Architecture (Fall 2018)
#
#  Project #3: Image Pixelization
#
#  November 6, 2018
#
#  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
#  Systems Software & Architecture Laboratory
#  Dept. of Computer Science and Engineering
#  Seoul National University
#  http://csl.snu.ac.kr
#
#--------------------------------------------------------------

.text
	.align 4
.globl bmp_mosaic
	.type bmp_mosaic,@function

bmp_mosaic:

	#------------------------------------------------------------
	# Use %rax, %rbx, %rcx, %rdx, %rsi, and %rdi registers only
	#	imgptr is in %rdi
	#	width  is in %rsi
	#	height is in %rdx
	#	size   is in %rcx
	#------------------------------------------------------------
    pushq %rbx
    movq %rsi, %rbx
    andq $3, %rbx
    leaq (%rsi, $rsi, 2), %rsi
    addq %rbx, %rsi
    # rax=?, rbx=width, rcx=size, rdx=height, rsi=bitWidth, rdi=imgPtr
    pushq %rdi
    xchq %rdi, %rdx
    # rax=?, rbx=width, rcx=size, rdx=, rsi=bitWidth, rdi=height
    # stack: [%rbx, %imgPtr
    movq %rbx, %rax
    divq %rcx
    pushq %rax
    movq %rdi, %rax
    divq %rcx
    pushq %rax
    # rax=?, rbx=width, rcx=size, rdx=, rsi=bitWidth, rdi=height
    # stack: [%rbx, %imgPtr, I_MAX, J_MAX
    movq $0, %rax
    pushq %rsi
    # rax=I, rbx=width, rcx=size, rdx=J, rsi=K, rdi=height
    # stack: [%rbx, %imgPtr, I_MAX, J_MAX, bitWidth
    LOOP_I:
        movq $0, %rdx
        LOOP_J:
            movq $0, %rsi
            LOOP_K:
                incq %rsi
                cmpq %rsi, $3
                jl LOOP_J
            incq %rdx
            cmpq %rdx, 4(%rsp)
            jle LOOP_I
        incq %rax
        cmpq %rdx, 8(%rsp)
        jle LOOP_I

CHANGE_PIXELS:
    pushq %rbx

    pushq %rax
    pushq %rdx
    # rax=i, rbx=w, rcx=sum, rdx=j, rsi=bitWidth, rdi=h


    popq %rax
    popq %rdx
	#------------------------------------------------------------
	ret

