/*
 * Copyright (c) 2018 William Pitcock <nenolod@dereferenced.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * This software is provided 'as is' and without any warranty, express or
 * implied.  In no event shall the authors be liable for any damages arising
 * from the use of this software.
 */
 
#undef __i686

.globl __start_context;
__start_context:
	/* get the proper context into position and test for NULL */
	leal	(%esp,%ebx,4), %esp
	cmpl	$0, (%esp)

	/* restore global offset table, exit@plt and __setcontext@plt need this */
	call	__i686.get_pc_thunk.bx
	addl	$_GLOBAL_OFFSET_TABLE_, %ebx

	/* if we have no linked context, lets get out of here */
	je	no_linked_context

	/* call setcontext to switch to the linked context */
	call	__setcontext@plt
	movl	%eax, (%esp)

no_linked_context:
	/* we are returning into a null context, it seems, so maybe we should exit */
	call	exit@plt

	/* something is really hosed, call hlt to force termination */
	hlt


__i686.get_pc_thunk.bx:
	mov	(%esp), %ebx
	ret
