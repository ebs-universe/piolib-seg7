

#ifndef _SEG7_H
#define _SEG7_H

#include <stdint.h>
#include <stdbool.h>
#include <platform/debug.h>
#include "config.h"

#if SEG7_SUPPORT_PRINTF
#include <printf/printf.h>
#endif

typedef uint8_t seg7_pos_spec_t;

/**
 * POSSPEC for number:
 *   punctuate | align | partial | - | 4b addr 
 *
 * ADDR is 
 *   - if punctuate, address for punctuation, from right end of unpadded number
 *   - if partial, start address if left aligned
 *   - if partial, end address for right aligned, 
 */
typedef enum {
    SEG7_POSSPEC_LEFTALIGN  = (1U << 6),
    SEG7_POSSPEC_RIGHTALIGN = (0U),
    SEG7_POSSPEC_PUNCTUATE  = (1U << 7),
    SEG7_POSSPEC_PARTIAL    = (1U << 5),
    SEG7_POSSPEC_FULL       = (0U),
    SEG7_POSSPEC_ADDR_MASK  = 0x0F
} seg7_posspec_bits;

/* The segment map:
 *     A
 *    ---
 * F |   | B
 *    -G-
 * E |   | C
 *    ---
 *     D
 * The enumeration can be extended with user symbols. 
 * For the 2nd (counting from left, [12:34]) digit of the display
 * the X bit controls the dots.
 */
typedef enum {    
            // XGFEDCBA
    SEG7_0 = 0b00111111,
    SEG7_1 = 0b00000110,
    SEG7_2 = 0b01011011,
    SEG7_3 = 0b01001111,
    SEG7_4 = 0b01100110,
    SEG7_5 = 0b01101101,
    SEG7_6 = 0b01111101,
    SEG7_7 = 0b00000111,
    SEG7_8 = 0b01111111,
    SEG7_9 = 0b01101111,
    SEG7_A = 0b01110111,
    SEG7_b = 0b01111100,
    SEG7_C = 0b00111001,
    SEG7_d = 0b01011110,
    SEG7_E = 0b01111001,
    SEG7_F = 0b01110001,
    SEG7_H = 0b01110110,
    SEG7_h = 0b01110100,
    SEG7_I = 0b00110000,
    SEG7_L = 0b00111000,
    SEG7_n = 0b01010100,
    SEG7_o = 0b01011100,
    SEG7_P = 0b01110011,
    SEG7_q = 0b01100111,
    SEG7_r = 0b00110001,
    SEG7_U = 0b00111110,
    SEG7_y = 0b01101110,
    SEG7_MINUS  = 0b01000000,
    SEG7_UNDER  = 0b00001000,
    SEG7_ABOVE  = 0b00000001,
    SEG7_DEGREE = 0b01100011,
    SEG7_DOTS   = 0b10000000,
    SEG7_CLEAR  = 0b00000000,
} seg7_symbol_t;

typedef struct SEG7_CHAR_t {
    char key;
    uint8_t value;
} seg7_char_t;

extern const seg7_symbol_t seg7_digits[0x10];

uint8_t seg7_prepbuf_number(uint8_t * buffer, uint8_t buffer_len, int16_t number, seg7_pos_spec_t pos_spec);

#if SEG7_SUPPORT_ALPHABET
extern const seg7_char_t seg7_alphabet[SEG7_ALPHABET_LEN];
#endif

#if SEG7_SUPPORT_PRINTF

int seg7_vprintf(char * buffer, uint8_t buffer_len, const char *format, va_list args);

static inline int seg7_printf(char * buffer, uint8_t buffer_len, const char *format, ...){
    uint8_t rval;
    va_list args;
    va_start( args, format );
    rval = seg7_vprintf(buffer, buffer_len, format, args);
    va_end(args);       
    return rval;
}

#endif
#endif
