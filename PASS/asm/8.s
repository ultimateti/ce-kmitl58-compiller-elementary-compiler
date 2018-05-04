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
	.string	"if we want to print in hex?\n"
.LC4:
	.string	"It is"
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
	subq	$16, %rsp
	subq	$8, %rsp
	movl	$23, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -8(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movl	$.LC3, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
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
	movl	$.LC1, %edi
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
