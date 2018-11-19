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
    # bitWidth calculated
    movq %rsi, %rax
    mulq %rdx
    addq %rdi, %rax
    subq %rsi, %rax
    DEBUG1:
    # rax=lastRow, %rbx=?, %rcx=size, %rdx=?, %rsi=bitWidth, %rdi=imgPtr
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
        subq (%rsp), %rdi
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
    pushq %rsi # push %rsi
    movq 32(%rsp), %rsi
    cmpq %rsi, %rcx
    cmovl %rcx, %rsi # assign size to new_height
    pushq %rcx
    # rax=?, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size
    movq $0, %rax
    LOOP_k:
        pushq %rdi
        addq %rax, %rdi
        # rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
        # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size, imgPtr
        jmp BLUR_ONE_CHANNEL
        BLUR_ONE_CHANNEL_END:
        incq %rax
        popq %rdi
        cmpq $3, %rax
        jl LOOP_k
    # rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size
    popq %rcx
    popq %rsi
    popq %rbx
    jmp BLUR_SQUARE_END



# rax=k, %rbx=new_width, %rcx=?, %rdx=?, %rsi=new_height, %rdi=imgPtr
# stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer, J Counter, bitWidth, size, imgPtr
BLUR_ONE_CHANNEL:
    pushq %rax
    pushq %rbx
    pushq %rsi
    # rax=?, %rbx=x counter, %rcx=y counter, %rdx=?, %rsi=sum, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer,
    # J Counter, bitWidth, size, imgPtr, k, new_width, new_height
    movq $0, %rsi
    movq $0, %rbx
    LOOP_X_SUM:
        movq $0, %rcx
        pushq %rdi
        LOOP_Y_SUM:
            movb (%rdi), %al
            addq %rax, %rsi
            addq $3, %rdi
            incq %rcx
            cmpq 8(%rsp), %rcx
            jl LOOP_Y_SUM
        popq %rdi
        subq 40(%rsp), %rdi # subtract bitWidth
        incq %rbx
        cmpq 16(%rsp), %rbx
        jl LOOP_X_SUM
    # rax=?, %rbx=x counter, %rcx=y counter, %rdx=?, %rsi=sum, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer,
    # J Counter, bitWidth, size, imgPtr, k, new_width, new_height, imgPtr
    movq %rbx, %rax
    mulq %rcx
    xchg %rax, %rsi
    movq $0, %rdx
    divq %rsi
    # rax=mean, %rbx=x counter, %rcx=y counter, %rdx=?, %rsi=?, %rdi=imgPtr
    # stack: [%rbx, width, (decreased)height, MAX_J, bitWidth*size, rowPointer,
    # J Counter, bitWidth, size, imgPtr, k, new_width, new_height, imgPtr
    movq $0, %rbx
    LOOP_X_ASSIGN:
        movq $0, %rcx
        addq 40(%rsp), %rdi # add bitWidth
        pushq %rdi
        LOOP_Y_ASSIGN:
            movb $255, (%rdi)
            addq $3, %rdi
            incq %rcx
            cmpq 8(%rsp), %rcx
            jl LOOP_Y_ASSIGN
        popq %rdi
        incq %rbx
        cmpq 16(%rsp), %rbx
        jl LOOP_X_ASSIGN
    popq %rsi
    popq %rbx
    popq %rax
    jmp BLUR_ONE_CHANNEL_END

