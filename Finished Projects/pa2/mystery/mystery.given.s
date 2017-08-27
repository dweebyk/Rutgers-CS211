	.file	"mystery.c"
	.comm	num,1600,32  #1600 bytes / 8 bytes per int = space for 200 ints
	.text
	.globl	add
	.type	add, @function
add:		#adding two values passed in.  I don't know what these registers are doing though
.LFB2:
	pushq	%rbp
	movq	%rsp, %rbp
	movq	%rdi, -8(%rbp)
	movq	%rsi, -16(%rbp)
	movq	-16(%rbp), %rax		#this is a useless line
	movq	-8(%rbp), %rdx		#useless line
	addq	%rdx, %rax		#add the passed values and return the sum
	popq	%rbp
	ret
.LFE2:
	.size	add, .-add
	.globl	dothething
	.type	dothething, @function
dothething:
.LFB3:
	pushq	%rbp
	movq	%rsp, %rbp
	pushq	%rbx
	subq	$40, %rsp		#make room for 40 bytes on stack
	movq	%rdi, -40(%rbp)		#this refers to the argument
	movq	-40(%rbp), %rax  	#argument is put to return
	movq	num(,%rax,8), %rax 	#.comm directives seem to me to store something like arrays??
	cmpq	$-1, %rax          	#if something in the array is negative?? 
	je	.L4			
	movq	-40(%rbp), %rax		#push the thing in memory into the return register
	movq	num(,%rax,8), %rax	#that number is to be put at num[number]
	jmp	.L5
.L4:
	movq	$-1, -24(%rbp)		#put -1 in memory
	cmpq	$0, -40(%rbp)		#compare argument with 0
	jne	.L6			#jump to 41 if not 0
	movq	$0, -24(%rbp)		#if not, move 0 into earlier memory and jump
	jmp	.L7
.L6:
	cmpq	$1, -40(%rbp)		#jump if the argument was 1
	jne	.L8		
	movq	$1, -24(%rbp)		#move 1 into memory and then jump
	jmp	.L7
.L8:					
	movq	-40(%rbp), %rax		#put argument into return register
	subq	$2, %rax		#some arg-2
	movq	%rax, %rdi		#move back into argument register
	call	dothething		#recursive call
	movq	%rax, %rbx		#move the returned value into rbx
	movq	-40(%rbp), %rax		#original value into return register
	subq	$1, %rax		#arg-1
	movq	%rax, %rdi		#move into argument register
	call	dothething		#recursive call
	movq	%rbx, %rsi		#move dothething(arg - 1) into first argument register
	movq	%rax, %rdi		#move dothethign(arg - 2) into second argument register
	call	add			#adding f(n-1) + f(n-2) is the definition of the fibonacci sequeunce
	movq	%rax, -24(%rbp)		#move the results of add into memory
.L7:
	movq	-40(%rbp), %rax		#move orginal argument into rax
	movq	num(,%rax,8), %rax	#move num[argument] into rax
	cmpq	$-1, %rax		#jump if it's not negative
	jne	.L9			
	movq	-40(%rbp), %rax		#move original argument into rax
	movq	-24(%rbp), %rdx		#move memory into register
	movq	%rdx, num(,%rax,8)	#move that memory into the array
.L9:
	movq	-40(%rbp), %rax		#move argument into return register
	movq	num(,%rax,8), %rax	#...then move what's in the num[argument] into return reg
.L5:
	addq	$40, %rsp		#break down stack and function prologue
	popq	%rbx
	popq	%rbp
	ret
.LFE3:
	.size	dothething, .-dothething
	.section	.rodata
.LC0:
	.string	"Value:   %d\n" #ahh, this is how you print out stuff
	.text
	.globl	main
	.type	main, @function
main:
.LFB4:
	pushq	%rbp			#preamble
	movq	%rsp, %rbp
	subq	$32, %rsp 		#4 bytes of space
	movl	%edi, -20(%rbp) 	#copy argc
	movq	%rsi, -32(%rbp)		#copy argv
	movq	-32(%rbp), %rax 	#put argv into return register
	addq	$8, %rax  		#add 8 to it??
	movq	(%rax), %rax		#OH this is first argument and i"m adding to the pointer
	movq	%rax, %rdi		#put the first argument of argv into argument register 
	call	atoi			#parse number from it
	movl	%eax, -8(%rbp)		#take 32 bit output and put it on stack
	movl	$0, -4(%rbp)		#move 0 under that output (I now know it's an iterator)
	jmp	.L11			#jump
.L12:
	movl	-4(%rbp), %eax		#take 0 and put it in return register
	cltq				#make int_32 into 64 bit int
	movq	$-1, num(,%rax,8)	#put -1 into array[inputnumber]
	addl	$1, -4(%rbp)		#ITERATION AND CHECKING.  IT MUST BE A LOOP
.L11:
	cmpl	$199, -4(%rbp)		#check for iterator less than 0
	jle	.L12			#if not, loop back
	movl	-8(%rbp), %eax		#put argument into return register
	cltq				#make int_32 into 64 bit int
	movq	%rax, %rdi		#put that 64 bit int into argument register
	call	dothething		#call dothething on that argument
	movq	%rax, %rsi		#put result into second argument register
	movl	$.LC0, %edi		#setting up for printf call
	movl	$0, %eax		#setting up to return 0
	call	printf			#yup; prints fibonacci
	movl	$0, %eax		#sets up for the function to return 0
	leave				#function prologue
	ret
.LFE4:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-4)"
	.section	.note.GNU-stack,"",@progbits
