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
	.string	"Compiler is so easy!!!\n"
.LC4:
	.string	"print _NN in hex : "
.LC5:
	.string	"\nprint _NN in dec : "
.LC6:
	.string	"\n-----So cool!----\n"
.LC7:
	.string	"*"
.LC8:
	.string	"\n"
.LC9:
	.string	"\nEND.....oh not!!!\n"
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
	subq	$32, %rsp
	movl	$.LC3, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	subq	$8, %rsp
	movl	$17, %eax
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$10, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	negq	 %rax
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
	movq	%rax, -8(%rbp)
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
	movl	$.LC1, %edi
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
	subq	$8, %rsp
	movl	$27, %eax
	movq	%rax, (%rsp)
	movq	-8(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	subq	(%rsp), %rax
	movq	%rax, (%rsp)
	cmpq	$0, (%rsp)
	jne	.L0
	movl	$.LC6, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
.L0:
	addq	$8, %rsp
	subq	$8, %rsp
	movl	$1, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -16(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	pushq	%rcx
	movq	-16(%rbp), %rcx
	jmp	.L1
.L2:
	movq	-16(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -24(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	pushq	%rcx
	movq	-24(%rbp), %rcx
	jmp	.L3
.L4:
	movl	$.LC7, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
	addq	$1, -24(%rbp)
.L3:
	cmpq	$5, %rcx
	jl	.L4
	popq	%rcx
	movl	$.LC8, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	subq	$8, %rsp
	movl	$1, %eax
	movq	%rax, (%rsp)
	movq	-16(%rbp), %rax
	subq	$8, %rsp
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
	addq	$1, %rcx
	addq	$1, -16(%rbp)
.L1:
	cmpq	$5, %rcx
	jl	.L2
	popq	%rcx
	movl	$.LC9, %esi
	movl	$.LC2, %edi
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
