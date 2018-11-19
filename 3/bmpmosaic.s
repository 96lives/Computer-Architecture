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
    pushq %rsi
    pushq %rdx
    movq %rsi, %rbx
	andq $3, %rbx
	leaq (%rsi, %rsi, 2), %rsi
    addq %rbx, %rsi
    movq %rsi, %rax
    mulq %rdx
    addq %rdi, %rax
    subq %rsi, %rax
    # rax=lastRow, %rbx=?, %rcx=size, %rdx=height, %rsi=bitWidth, %rdi=imgPtr
    # stack: [%rbx, width, height
    movq %rax, %rdi
    movq $0, %rdx
    movq 8(%rsp), %rax #width to %rax
    divq %rcx
    pushq %rax
    movq %rsi, %rax
    mulq %rcx
    pushq %rax
    # rax=bitWidth*size, %rbx=?, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=lastRow
    # stack: [%rbx, width, height, MAX_J, bitWidth*size
    # TESTED
    LOOP_I:
        pushq %rdi #push first column of the row
        movq $0, %rbx
        # rax=?, %rbx=J counter, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
        # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer
        LOOP_J:
            jmp BLUR_SQUARE
            BLUR_SQUARE_END:
            leaq (%rcx, %rcx, 2), %rax
            addq %rax, %rdi
            incq %rbx
            cmpq 16(%rsp), %rbx
            jle LOOP_J
        # rax=?, %rbx=?, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
        # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer
        movq 24(%rsp), %rax
        subq %rcx, %rax
        jl END
        movq %rax, 24(%rsp)
        popq %rdi
        subq 8(%rsp), %rdi
        jmp LOOP_I
    # rax=?, %rbx=?, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer
    END:
    addq $40, %rsp
    popq %rbx
ret

# rax=?, %rbx=J counter, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
# stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer
BLUR_SQUARE:
    # calculate width
    pushq %rbx # push J Counter
    movq %rbx, %rax
    mulq %rcx
    movq 40(%rsp), %rbx
    subq %rbx, %rax
    cmpq %rbx, %rcx
    cmovl %rcx, %rbx # assign size to new_width
    # rax=?, %rbx=new_width, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter
    pushq %rsi # push %rsi TODO: Weird
    movq 32(%rsp), %rsi
    cmpq %rsi, %rcx
    cmovl %rcx, %rsi # assign size to new_height
    pushq %rcx
    # CALCULATE SUMS
    # rax=?, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size
    movq $0, %rax
    LOOP_k:
        pushq %rdi
        addq %rax, %rdi
        pushq $0
        # rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
        # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size, imgPtr, sum
        jmp BLUR_ONE_CHANNEL
        BLUR_ONE_CHANNEL_END:
        incq %rax
        movq 8(%rsp), %rdi
        addq $16, %rsp
        cmpq $3, %rax
        jl LOOP_k
    # rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size
    popq %rcx
    popq %rsi
    popq %rbx
    jmp BLUR_SQUARE_END



# rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
# stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size, imgPtr, sum
BLUR_ONE_CHANNEL:
    

    jmp BLUR_ONE_CHANNEL_END

