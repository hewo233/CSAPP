	.file	"fib.c"
	.text
	.globl	fib
	.type	fib, @function
fib:
.LFB0:
	.cfi_startproc
	movl	$1, %eax
	movl	$0, %edx
	jmp	.L2
.L3:
	leal	(%rdx,%rax), %ecx
	subl	$1, %edi
	movl	%eax, %edx
	movl	%ecx, %eax
.L2:
	testl	%edi, %edi
	jg	.L3
	ret
	.cfi_endproc
.LFE0:
	.size	fib, .-fib
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
