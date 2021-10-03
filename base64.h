#ifndef BASE64_H
#define BASE64_H 1

#include <stddef.h>

void base64_encode(char *dst, const unsigned char *src, size_t len);
size_t base64_decode(unsigned char *dst, const char *src);

#endif
