

#ifndef SEG7_CONFIG_H
#define SEG7_CONFIG_H

#include <application.h>

#ifndef SEG7_MAX_LEN
#define SEG7_MAX_LEN            8
#endif

#ifndef SEG7_SUPPORT_PRINTF
#define SEG7_SUPPORT_PRINTF     1
#endif

#ifndef SEG7_SUPPORT_ALPHABET
#define SEG7_SUPPORT_ALPHABET   SEG7_SUPPORT_PRINTF
#endif

#ifndef SEG7_INCLUDE_CHAR_E
#define SEG7_INCLUDE_CHAR_E     SEG7_SUPPORT_PRINTF
#endif

#define SEG7_ALPHABET_LEN       (SEG7_INCLUDE_CHAR_E)

#endif