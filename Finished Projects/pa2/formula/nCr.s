
.globl nCr
	.type	nCr, @function
nCr:
  # Your code for nCr should go here
	pushq	%rbp			# function preamble
	movq	%rsp, %rbp
	pushq	%rdx			#used for idiv, preserve value by pushing to stack
	pushq	%rbx			#used to hold (n-r)!r!, preserve value by pushing to stack
	subq	$16, %rsp		#space for 2 integers

	movq	%rdi, (%rsp)		#copy n onto the stack
	subq	%rsi, (%rsp)		#copy n-r into onto the stack
	
	call	Factorial
	cmpl	$0, %eax		#see if overflow occurred in Factorial
	je	.checkoverflow		#return 0 if so
	movl	%eax, %ebx		#copy n! into ebx (for later use in idiv)

	movl	%esi, %edi		#copy r into edi now as the primary parameter
	call	Factorial		
	cmpl	$0, %eax		#check for overflow again
	je	.checkoverflow
	movq	%rax, 8(%rsp)		#copy r! into first space for int on stack


	movq	(%rsp), %rdi		#copy n-r into edi as primary parameter
	call	Factorial		
	cmpl	$0, %eax
	je	.checkoverflow

	imulq	8(%rsp), %rax		#multiply eax = (n-r)! and r! and stuff into %eax
	jo	.checkoverflow

	movq	%rax, 8(%rsp)		#overwrite r! with product of denominator
	movl	%ebx, %eax		#put n! into eax
	movq	$0, %rdx		#clear rdx register
	movq	8(%rsp), %rbx		#move product of denominator into rbx
	idivl	%ebx			#divide eax = n! by ebx = r!(n-r)! and result is in eax
	jo	.checkoverflow
	jmp	.endnCr
.checkoverflow:
	movq	$0, %rax
.endnCr:
	addq	$16, %rsp		#move up the stack past the arguments so we can pop rbx and rdx
	popq	%rbx
	popq	%rdx			#restore the original value of rdx 

	movq	%rbp, %rsp		#function prologue
	popq	%rbp
	ret
	


#I use only registers here, so I can actually do this without function preamble and function exit on the stack

.globl Factorial
	.type	Factorial, @function
Factorial:
  # Your code for Factorial should go here
	cmpl	$1, %edi		#arg from edi
	jle	.basecase		#jump to return 1
	movl	%edi, %eax		#move current n into eax

.recursion:				#not really recursion...but let me have my fun :<
	leal	-1(%edi), %edi		#in c code: n = n - 1
	imull	%edi, %eax		#multiply two values and stuff into eax
	jo	.overflow		#if overflow, return 0
	cmpl	$1, %edi		#see if parameter is one
	je	.endFact		#if so, end the fuction
	jmp	.recursion		#else, multiply by (n-1)!

.overflow:
	movl	$0, %eax		#put 0 to return
	jmp	.endFact
.basecase:
	movl	$1, %eax		#return 1 if edi is already 1
.endFact:
	ret	
	
