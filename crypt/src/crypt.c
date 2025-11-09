#include <unistd.h>
#include "crypt_i.h"

char *crypt(const char *key, const char *salt)
{
	/* This buffer is sufficiently large for all
	 * currently-supported hash types. It needs to be updated if
	 * longer hashes are added. The cast to struct crypt_data * is
	 * purely to meet the public API requirements of the crypt_r
	 * function; the implementation of crypt_r uses the object
	 * purely as a char buffer. */
	static struct crypt_data buf;
	return __crypt_r(key, salt, &buf);
}
