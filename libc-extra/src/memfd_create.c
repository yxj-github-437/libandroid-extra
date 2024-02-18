#include <sys/mman.h>
#include <syscall.h>
#include <unistd.h>

int memfd_create(const char* name, unsigned flags)
{
    return syscall(SYS_memfd_create, name, flags);
}
