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




bmp_grid:
	#------------------------------------------------------------
	# Use %rax, %rbx, %rcx, %rdx, %rsi, and %rdi registers only
	#	imgptr is in %rdi
	#	width  is in %rsi
	#	height is in %rdx
	#   gap	   is in %rcx
	#------------------------------------------------------------

	movq $0, (%rdi)
	movq $0, 1(%rdi)
	movq $255, 2(%rdi)
	movq $0, 3(%rdi)
	movq $0, 4(%rdi)
	movq $255, 5(%rdi)
	movq $0, 6(%rdi)
	movq $0, 7(%rdi)
	movq $255, 8(%rdi)
	movq $0, 9(%rdi)
	movq $0, 10(%rdi)
	movq $255, 11(%rdi)




	movq %rdi, %rax















	#------------------------------------------------------------

	ret
