#include <stdio.h>
#include <alloca.h>
#include <string.h>
#include <iconv.h>

int main() {
    const char *encTo = "GBK";
    const char *encFrom = "UTF-8";
    iconv_t cd = iconv_open(encTo, encFrom);
    if (cd == (iconv_t)-1) {
        perror("iconv_open");
        return -1;
    }

    char inbuf[1024] = "iconv 测试";
    size_t inbufLen = strlen(inbuf);
    size_t outbufLen = 1024;
    char* outbuf = alloca(outbufLen);
    memset(outbuf, 0, outbufLen);
    char *inbufSrc = inbuf;
    char *outbufSrc = outbuf;

    size_t ret = iconv(cd, (char **)&inbufSrc, &inbufLen, (char **)&outbufSrc, &outbufLen);
    if (ret == (size_t)-1) {
        perror("iconv");
        iconv_close(cd);
        return -1;
    }

    printf("%s\n", outbuf);

    iconv_close(cd);
    return 0;
}
