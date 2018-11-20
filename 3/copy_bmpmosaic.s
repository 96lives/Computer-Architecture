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
    movq %rsi, %rax
    movq %rsi, %rbx
    andq $3, %rbx
    leaq (%rsi, %rsi, 2), %rsi
    addq %rbx, %rsi
    movq %rax, %rbx
    # rax=width, rbx=width, rcx=size, rdx=height, rsi=bitWidth, rdi=imgPtr
    # stack = [%rbx,
    pushq %rdx
    movq $0, %rdx
    idivq %rcx
    pushq %rax
    movq %rsi, %rax
    mulq 8(%rsp)
    pushq %rax
    pushq %rbx
    # rax=?, rbx=width, rcx=size, rdx=J, rsi=bitWidth, rdi=imgPtr
    # stack = [%rbx, height, MAX_J, imgEnd, width
    LOOP_I:
        pushq %rdi
        movq $0, %rdx
        # rax=?, rbx=? rcx=size, rdx=J, rsi=bitWidth, rdi=imgPtr
        # stack = [%rbx, height, MAX_J, imgEnd, width, imgPtr
        LOOP_J:
            CALL change_pixels
            leaq (%rcx, %rcx, 2), %rax
            addq %rax, %rdi
            incq %rdx
            cmpq 16(%rsp), %rdx
            jle LOOP_J
        popq %rdi
        movq 20(%rsp), %rax
        subq %rcx, %rax
        movq 20(%rsp), %rax
        addq %rsi, %rdi
        cmpq %rdi, 4(%rsp)
        jl LOOP_I
    popq %rsi
    popq %rax
    popq %rax
    popq %rdx
    popq %rbx
	ret

# rax=?, rbx=?, rcx=size, rdx=J, rsi=bitWidth, rdi=imgPtr
# stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width, imgPtr
change_pixels:
    movq 20(%rsp), %rax
    cmpq %rcx, %rax
    cmovge %rcx, %rax
    pushq %rdx
    xchg %rdx, %rax
    # rax=J, rbx=?, rcx=size, rdx=new_height, rsi=bitWidth, rdi=imgPtr
    # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width, imgPtr, J
    mulq %rcx
    movq 12(%rsp), %rbx
    subq %rax, %rbx
    cmovge %rcx, %rbx
    pushq %rcx
    pushq %rbx
    pushq %rdx
    pushq %rsi
    # rax=k,rbx=i, rcx=j, rdx=sum, rsi=?, rdi=imgPtr
    # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
    #..., imgPtr, J, size, new_width, new_height, bitWidth
    movq $0, %rax
    movq $0, %rdx
    LOOP_k:
        movq $0, %rbx
        pushq %rdi
        LOOP_i_sum:
            movq $0, %rcx
            pushq %rdi
            LOOP_j_sum:
                # rax=k,rbx=i, rcx=j, rdx=sum, rsi=?, rdi=imgPtr
                # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
                #..., imgPtr, J, size, new_width, new_height, bitWidth, imgPtr(K), imgPtr(J)
                addq (%rdi), %rdx
                addq $3, %rdi
                incq %rcx
                cmpq 20(%rsp), %rcx
                jl LOOP_j_sum
            popq %rdi
            addq 8(%rsp), %rdi
            incq %rbx
            cmpq 12(%rsp), %rbx
            jl LOOP_i_sum

        # rax=k,rbx=i, rcx=j, rdx=sum, rsi=?, rdi=imgPtr
        # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
        #..., imgPtr, J, size, new_width, new_height, bitWidth, imgPtr(K)
        pushq %rax
        movq 16(%rsp), %rax
        movq 12(%rsp), %rbx
        mulq %rbx
        xchg %rdx, %rax
        movq %rdx, %rsi
        movq $0, %rdx
        divq %rsi
        popq %rdx
        xchg %rdx, %rax
        # rax=k,rbx=i, rcx=j, rdx=mean_value, rsi=?, rdi=imgPtr
        # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
        #..., imgPtr, J, size, new_width, new_height, bitWidth, imgPtr(K)
        movq 4(%rsp), %rdi
        movq $0, %rbx
        #pushq %rdi
        LOOP_i_assign:
            movq $0, %rcx
            pushq %rdi
            LOOP_j_assign:
                # rax=k,rbx=i, rcx=j, rdx=mean_value, rsi=?, rdi=imgPtr
                # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
                #..., imgPtr, J, size, new_width, new_height, bitWidth, imgPtr(K), imgPtr(J)
                movb %dl, (%rdi)
                addq $3, %rdi
                incq %rcx
                cmpq 20(%rsp), %rcx
                jl LOOP_j_assign
            popq %rdi
            addq 8(%rsp), %rdi
            incq %rbx
            cmpq 12(%rsp), %rbx
            jl LOOP_i_assign
        popq %rdi
        incq %rdi
        incq %rax
        cmpq $3, %rax
        jl LOOP_k
    # rax=k,rbx=i, rcx=j, rdx=mean_value, rsi=?, rdi=imgPtr
    # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width,
    #..., imgPtr, J, size, new_width, new_height, bitWidth
    subq $3, %rdi
    popq %rsi
    popq %rax
    popq %rax
    popq %rcx
    popq %rdx
    # rax=?, rbx=?, rcx=size, rdx=J, rsi=bitWidth, rdi=imgPtr
    # stack = [%rbx, (subtracted)height, MAX_J, imgEnd, width, imgPtr
    ret

