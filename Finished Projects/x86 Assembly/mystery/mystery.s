	.file	"mystery.c"
	.text
	.globl	add
	.type	add, @function
add:
.LFB28:
	.cfi_startproc
	leal	(%rdi,%rsi), %eax
	ret
	.cfi_endproc
.LFE28:
	.size	add, .-add
	.globl	dothething
	.type	dothething, @function
dothething:
.LFB29:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	subq	$8, %rsp
	.cfi_def_cfa_offset 32
	movl	%edi, %ebx
	movslq	%edi, %rdx
	leaq	num(%rip), %rax
	cmpl	$-1, (%rax,%rdx,4)
	je	.L7
.L3:
	movslq	%ebx, %rbx
	leaq	num(%rip), %rax
	movl	(%rax,%rbx,4), %eax
	addq	$8, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	ret
.L7:
	.cfi_restore_state
	testl	%edi, %edi
	je	.L8
	cmpl	$1, %edi
	je	.L9
	leal	-2(%rdi), %edi
	call	dothething
	movl	%eax, %ebp
	leal	-1(%rbx), %edi
	call	dothething
	movslq	%ebx, %rdx
	addl	%eax, %ebp
	leaq	num(%rip), %rax
	movl	%ebp, (%rax,%rdx,4)
	jmp	.L3
.L8:
	movl	$0, num(%rip)
	jmp	.L3
.L9:
	movl	$1, 4+num(%rip)
	jmp	.L3
	.cfi_endproc
.LFE29:
	.size	dothething, .-dothething
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Value:\t%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB30:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movq	8(%rsi), %rdi
	movl	$10, %edx
	movl	$0, %esi
	call	strtol@PLT
	leaq	num(%rip), %rdx
	leaq	800+num(%rip), %rcx
.L11:
	movl	$-1, (%rdx)
	addq	$4, %rdx
	cmpq	%rcx, %rdx
	jne	.L11
	movl	%eax, %edi
	call	dothething
	movl	%eax, %esi
	leaq	.LC0(%rip), %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE30:
	.size	main, .-main
	.comm	num,800,32
	.ident	"GCC: (Debian 6.3.0-18) 6.3.0 20170516"
	.section	.note.GNU-stack,"",@progbits
