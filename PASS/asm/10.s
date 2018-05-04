	.file	"test.c"
	.globl	from
	.bss
	.align 8
	.type	from, @object
	.size	from, 8
from:
	.zero	8
	.section .rodata
.LC0:
	.string "%ld"
.LC1:
	.string "%Xh"
.LC2:
	.string "%s"
.LC3:
	.string	"Let's play with operator\n"
.LC4:
	.string	"Tid lob :D "
.LC5:
	.string	"\nPlus "
.LC6:
	.string	"\nMinus "
.LC7:
	.string	"\nMul "
.LC8:
	.string	"\nDivide "
.LC9:
	.string	"\nMod "
.LC10:
	.string	"\nAlso works with paren! "
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movl	$.LC3, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	subq	$8, %rsp
	movl	$123, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	negq	 %rax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -8(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rdx
	addq	$8, %rsp
	movq	(%rsp), %rax
	addq	$8, %rsp
	addq	%rdx, %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -16(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	subq	(%rsp), %rax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -24(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	imulq	 (%rsp), %rax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -32(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	cqto
	idivq	(%rsp)
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -40(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	cqto
	idivq	(%rsp)
	movq	%rdx, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -48(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$86, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$34, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$18, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rdx
	addq	$8, %rsp
	movq	(%rsp), %rax
	addq	$8, %rsp
	addq	%rdx, %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	imulq	 (%rsp), %rax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -56(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movl	$.LC4, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-8(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC5, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-16(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC6, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-24(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC7, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-32(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-40(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-48(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC10, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movq	-56(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, %rsi
	movl	$.LC0, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size main, .-main
	.ident "GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.6) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
