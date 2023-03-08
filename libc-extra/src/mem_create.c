#include <syscall.h>
#include <unistd.h>
#include <sys/mman.h>

int memfd_create(const char *name, unsigned flags)
{
	return syscall(SYS_memfd_create, name, flags);
}