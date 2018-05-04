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
	.string	"I want to draw somthing!!!\n"
.LC4:
	.string	"Butterfly?\n"
.LC5:
	.string	"*"
.LC6:
	.string	" "
.LC7:
	.string	"*"
.LC8:
	.string	"\n"
.LC9:
	.string	"*"
.LC10:
	.string	" "
.LC11:
	.string	"*"
.LC12:
	.string	"\n"
.LC13:
	.string	"i hope it\'s a butterfly\n"
.LC14:
	.string	"END"
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
	subq	$40, %rsp
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
	subq	$8, %rsp
	movl	$5, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -8(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$1, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -16(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	subq	$8, %rsp
	movl	$0, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -24(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L0
.L1:
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L2
.L3:
	movl	$.LC5, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L2:
	cmpq	-16(%rbp), %rcx
	jl	.L3
	popq	%rcx
	subq	$8, %rsp
	movl	$2, %eax
	movq	%rax, (%rsp)
	movq	-16(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	-8(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	subq	(%rsp), %rax
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
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L4
.L5:
	movl	$.LC6, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L4:
	cmpq	-32(%rbp), %rcx
	jl	.L5
	popq	%rcx
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L6
.L7:
	movl	$.LC7, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L6:
	cmpq	-16(%rbp), %rcx
	jl	.L7
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
.L0:
	cmpq	-8(%rbp), %rcx
	jl	.L1
	popq	%rcx
	subq	$8, %rsp
	movl	$4, %eax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -16(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L8
.L9:
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L10
.L11:
	movl	$.LC9, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L10:
	cmpq	-16(%rbp), %rcx
	jl	.L11
	popq	%rcx
	subq	$8, %rsp
	movl	$2, %eax
	movq	%rax, (%rsp)
	movq	-16(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	-8(%rbp), %rax
	subq	$8, %rsp
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	subq	(%rsp), %rax
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
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L12
.L13:
	movl	$.LC10, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L12:
	cmpq	-32(%rbp), %rcx
	jl	.L13
	popq	%rcx
	pushq	%rcx
	movq	$0, %rcx
	jmp	.L14
.L15:
	movl	$.LC11, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	addq	$1, %rcx
.L14:
	cmpq	-16(%rbp), %rcx
	jl	.L15
	popq	%rcx
	movl	$.LC12, %esi
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
	movq	(%rsp), %rax
	addq	$8, %rsp
	subq	(%rsp), %rax
	movq	%rax, (%rsp)
	movq	(%rsp), %rax
	addq	$8, %rsp
	movq	%rax, -16(%rbp)
	subq	$8, %rsp
	movq	%rax, (%rsp)
	addq	$1, %rcx
.L8:
	cmpq	-8(%rbp), %rcx
	jl	.L9
	popq	%rcx
	movl	$.LC13, %esi
	movl	$.LC2, %edi
	movl	$0, %eax
	pushq	%rcx
	call	printf
	popq	%rcx
	movl	$.LC14, %esi
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
