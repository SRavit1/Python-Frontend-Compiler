	.text
	.file	"sq.cpp"
	.globl	_Z6squarei              # -- Begin function _Z6squarei
	.p2align	4, 0x90
	.type	_Z6squarei,@function
_Z6squarei:                             # @_Z6squarei
	.cfi_startproc
# %bb.0:
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset %rbp, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register %rbp
	movl	%edi, -4(%rbp)
	movl	-4(%rbp), %eax
	imull	-4(%rbp), %eax
	popq	%rbp
	retq
.Lfunc_end0:
	.size	_Z6squarei, .Lfunc_end0-_Z6squarei
	.cfi_endproc
                                        # -- End function

	.ident	"clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)"
	.section	".note.GNU-stack","",@progbits
