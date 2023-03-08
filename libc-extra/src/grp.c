#include <grp.h>

struct group* getgrent(void)
{
    return 0;
}

void setgrent(void)
{}
void endgrent(void)
{}

int getgrgid_r(gid_t gid, struct group* grp, char* buf, size_t buflen, struct group** result)
{
    *result = 0;
    return 0;
}

int getgrnam_r(const char* name, struct group* grp, char* buf, size_t buflen, struct group** result)
{
    *result = 0;
    return 0;
}