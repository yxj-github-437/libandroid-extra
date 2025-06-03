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

#define _GNU_SOURCE
#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/ucontext.h>


extern void __start_context(void);


void
__makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)
{
	unsigned long *sp;
	va_list va;
	int i;

	sp = (unsigned long *) ((uintptr_t) ucp->uc_stack.ss_sp + ucp->uc_stack.ss_size);
	sp -= argc < 8 ? 0 : argc - 8;
	sp = (unsigned long *) (((uintptr_t) sp & -16L));

	ucp->uc_mcontext.sp = (uintptr_t) sp;
	ucp->uc_mcontext.pc = (uintptr_t) func;
	ucp->uc_mcontext.regs[19] = (uintptr_t) ucp->uc_link;
	ucp->uc_mcontext.regs[30] = (uintptr_t) &__start_context;

	va_start(va, argc);

	for (i = 0; (i < argc && i < 8); i++)
		ucp->uc_mcontext.regs[i] = va_arg (va, unsigned long);

	for (; i < argc; i++)
		*sp++ = va_arg (va, unsigned long);

	va_end(va);
}


extern __typeof(__makecontext) makecontext __attribute__((weak, __alias__("__makecontext")));
