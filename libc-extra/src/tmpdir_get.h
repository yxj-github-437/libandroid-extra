#pragma once

#include <linux/limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>

static const char* tmpdir_get(char* buf, size_t len) {
    char* tmpdir = getenv("TMPDIR");
    if (tmpdir)
    {
        snprintf(buf, len, "%s", tmpdir);
    }
    else
    {
        getcwd(buf, len);
    }

    return buf;
}
