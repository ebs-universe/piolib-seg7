

#include "seg7.h"
#include <ctype.h>

const seg7_symbol_t seg7_digits[0x10] = {
//    XGFEDCBA
    0b00111111,  // 0
    0b00000110,  // 1
    0b01011011,  // 2
    0b01001111,  // 3
    0b01100110,  // 4
    0b01101101,  // 5
    0b01111101,  // 6
    0b00000111,  // 7
    0b01111111,  // 8
    0b01101111,  // 9
    0b01110111,  // A
    0b01111100,  // b
    0b00111001,  // C
    0b01011110,  // d
    0b01111001,  // E
    0b01110001,  // F
};

#if SEG7_SUPPORT_ALPHABET
const seg7_char_t seg7_alphabet[SEG7_ALPHABET_LEN] = {
    #if SEG7_INCLUDE_CHAR_E
    {.key = 'e', .value = SEG7_E},
    #endif
};
#endif

uint8_t seg7_prepbuf_number(uint8_t * buffer, uint8_t buffer_len, int16_t number, seg7_pos_spec_t pos_spec) {
    uint8_t cursor = buffer_len;
    uint8_t i;
    bool neg = false;

    if (pos_spec & SEG7_POSSPEC_PARTIAL) {
        // Not Implemented
        die();          
    }

    if (number < 0) {
        neg = true;
        number = -1 * number;   
    }

    if (number == 0) {
        cursor--;
        buffer[cursor] = SEG7_0;
    }

    while (number) {
        if (!cursor) {
            return 0;
        }
        cursor--;
        i = number % 10;
        buffer[cursor] = seg7_digits[i];        
        number /= 10;
    }

    if (neg) {
        if (!cursor) {
            return 0;
        }
        cursor--;
        buffer[cursor] = SEG7_MINUS;
    }

    uint8_t addr = pos_spec & SEG7_POSSPEC_ADDR_MASK;
    if (pos_spec & SEG7_POSSPEC_PUNCTUATE) {
        buffer[buffer_len - addr - 1] |= 0x80;
    } 
    
    uint8_t len = buffer_len - cursor;

    if (pos_spec & SEG7_POSSPEC_LEFTALIGN) {
        if (cursor > 0) {
            for (i = 0; i < buffer_len; i++) {
                if (i < len) {
                    buffer[i] = buffer[cursor + i];
                } else {
                    buffer[i] = SEG7_CLEAR;
                }
            }
        }
    } else {
        while (cursor)
        {
            cursor--;
            buffer[cursor] = SEG7_CLEAR;
        }
    }
    return 1;
}

static uint8_t _seg7_from_ascii(char ch) {
    if (ch >= '0' && ch <= '9') {
        return seg7_digits[ch - '0'];  // For digits 0-9
    }

    #if SEG7_SUPPORT_ALPHABET

    ch = tolower(ch);
    for (int i = 0; i < SEG7_ALPHABET_LEN; i++) {
        if (seg7_alphabet[i].key == ch) {
            return seg7_alphabet[i].value;
        }
    }
    
    #endif

    if (ch == '-') {
        return SEG7_MINUS;  
    }
    
    return 0;
}

#if SEG7_SUPPORT_PRINTF

uint8_t _seg7_buffer[SEG7_MAX_LEN + 2];

int seg7_vprintf(char * buffer, uint8_t buffer_len, const char *format, va_list args)
{
    void * s7ptr = &_seg7_buffer[0];
    uint8_t len = print( &s7ptr, PRINT_TTYPE_STRING, format, args );
    uint8_t i = 0, j = 0;

    while (i < len && j < buffer_len && _seg7_buffer[i] != 0x00) {
        if (_seg7_buffer[i] == '.') {
            if (j > 0) {
                buffer[j - 1] |= 0x80;
            }
        } else {
            buffer[j] = _seg7_from_ascii(_seg7_buffer[i]);
            j++;
        }
        i++;
    }

    return j;
}

#endif