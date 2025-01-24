

#include "seg7.h"

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

uint8_t seg7_prep_display_buffer(int16_t number, seg7_pos_spec_t pos_spec, uint8_t * buffer, uint8_t buffer_len) {
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
