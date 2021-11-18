#include "base64.h"

size_t
base64_encode(char *dst, const unsigned char *src, size_t len)
{
	static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	const char *start = dst; 
	for (size_t i = 0; i < len; i += 3, dst += 4) {
		unsigned long x = (src[i] & 0xfful) << 16;
		dst[3] = i + 2 >= len ? '=' : b64[(x |= src[i + 2] & 0xfful) & 0x3f];
		dst[2] = i + 1 >= len ? '=' : b64[(x |= (src[i + 1] & 0xfful) << 8) >> 6 & 0x3f];
		dst[1] = b64[x >> 12 & 0x3f];
		dst[0] = b64[x >> 18];
	}
	*dst = '\0';
	return (dst - start);
}

size_t
base64_decode(unsigned char *dst, const char *src)
{
	static const char b64[] = {
		['A'] =  0, ['B'] =  1, ['C'] =  2, ['D'] =  3,
		['E'] =  4, ['F'] =  5, ['G'] =  6, ['H'] =  7,
		['I'] =  8, ['J'] =  9, ['K'] = 10, ['L'] = 11,
		['M'] = 12, ['N'] = 13, ['O'] = 14, ['P'] = 15,
		['Q'] = 16, ['R'] = 17, ['S'] = 18, ['T'] = 19,
		['U'] = 20, ['V'] = 21, ['W'] = 22, ['X'] = 23,
		['Y'] = 24, ['Z'] = 25, ['a'] = 26, ['b'] = 27,
		['c'] = 28, ['d'] = 29, ['e'] = 30, ['f'] = 31,
		['g'] = 32, ['h'] = 33, ['i'] = 34, ['j'] = 35,
		['k'] = 36, ['l'] = 37, ['m'] = 38, ['n'] = 39,
		['o'] = 40, ['p'] = 41, ['q'] = 42, ['r'] = 43,
		['s'] = 44, ['t'] = 45, ['u'] = 46, ['v'] = 47,
		['w'] = 48, ['x'] = 49, ['y'] = 50, ['z'] = 51,
		['0'] = 52, 53, 54, 55, 56, 57, 58, 59, 60, 61,
		['+'] = 62, ['/'] = 63, ['='] =  0,
	};
	unsigned long x;
	size_t i, c, len, pad;

	for (i = 0, x = 0, len = 0, pad = 0; src[i]; ++i) {
		c = src[i];
		if (c == '=' && (!src[i + 1] || (src[i + 1] == '=' && !src[i + 2])))
			++pad;
		else if (c >= sizeof(b64) || (!b64[c] && c != 'A'))
			return 0;
		x = x << 6 | b64[c];
		if (i % 4 == 3) {
			dst[len + 2] = x & 0xff, x >>= 8;
			dst[len + 1] = x & 0xff, x >>= 8;
			dst[len + 0] = x & 0xff;
			len += 3;
		}
	}
	if (i % 4 != 0)
		return 0;
	return len - pad;
}
