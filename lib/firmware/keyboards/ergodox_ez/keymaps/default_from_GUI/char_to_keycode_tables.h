#ifndef _CHAR_TO_KEYCODE_TABLES_H_
#define _CHAR_TO_KEYCODE_TABLES_H_

#include <stdint.h>
#include <string.h>
#include "keycode.h"
#include "quantum_keycodes.h"

typedef struct {
  const uint32_t codepoint;
  const uint16_t keycode;
} char_to_keycode_t;

typedef struct {
  const uint32_t size;
  const char_to_keycode_t table[];
} c2k_table_t;

#ifdef LOAD_C2K_EN_US_AS
const c2k_table_t PROGMEM c2k_en_us = {
  .size = 97,
  .table = {
    {.codepoint = 0x9, .keycode = KC_TAB},                    // Horizontal Tab (\t)
    {.codepoint = 0xA, .keycode = KC_ENTER},                  // Line Feed (\n)
    {.codepoint = 0x20, .keycode = KC_SPACE},                 // Space
    {.codepoint = 0x21, .keycode = KC_EXCLAIM},               // !
    {.codepoint = 0x22, .keycode = KC_DOUBLE_QUOTE},          // "
    {.codepoint = 0x23, .keycode = KC_HASH},                  // #
    {.codepoint = 0x24, .keycode = KC_DOLLAR},                // $
    {.codepoint = 0x25, .keycode = KC_PERCENT},               // %
    {.codepoint = 0x26, .keycode = KC_AMPERSAND},             // &
    {.codepoint = 0x27, .keycode = KC_QUOTE},                 // '
    {.codepoint = 0x28, .keycode = KC_LEFT_PAREN},            // (
    {.codepoint = 0x29, .keycode = KC_RIGHT_PAREN},           // )
    {.codepoint = 0x2A, .keycode = KC_ASTERISK},              // *
    {.codepoint = 0x2B, .keycode = KC_PLUS},                  // +
    {.codepoint = 0x2C, .keycode = KC_COMMA},                 // ,
    {.codepoint = 0x2D, .keycode = KC_MINUS},                 // -
    {.codepoint = 0x2E, .keycode = KC_DOT},                   // .
    {.codepoint = 0x2F, .keycode = KC_SLASH},                 // /
    {.codepoint = 0x30, .keycode = KC_0},                     // 0
    {.codepoint = 0x31, .keycode = KC_1},                     // 1
    {.codepoint = 0x32, .keycode = KC_2},                     // 2
    {.codepoint = 0x33, .keycode = KC_3},                     // 3
    {.codepoint = 0x34, .keycode = KC_4},                     // 4
    {.codepoint = 0x35, .keycode = KC_5},                     // 5
    {.codepoint = 0x36, .keycode = KC_6},                     // 6
    {.codepoint = 0x37, .keycode = KC_7},                     // 7
    {.codepoint = 0x38, .keycode = KC_8},                     // 8
    {.codepoint = 0x39, .keycode = KC_9},                     // 9
    {.codepoint = 0x3A, .keycode = KC_COLON},                 // :
    {.codepoint = 0x3B, .keycode = KC_SCOLON},                // ;
    {.codepoint = 0x3C, .keycode = KC_LEFT_ANGLE_BRACKET},    // <
    {.codepoint = 0x3D, .keycode = KC_EQUAL},                 // =
    {.codepoint = 0x3E, .keycode = KC_RIGHT_ANGLE_BRACKET},   // >
    {.codepoint = 0x3F, .keycode = KC_QUESTION},              // ?
    {.codepoint = 0x40, .keycode = KC_AT},                    // @
    {.codepoint = 0x41, .keycode = LSFT(KC_A)},               // A
    {.codepoint = 0x42, .keycode = LSFT(KC_B)},               // B
    {.codepoint = 0x43, .keycode = LSFT(KC_C)},               // C
    {.codepoint = 0x44, .keycode = LSFT(KC_D)},               // D
    {.codepoint = 0x45, .keycode = LSFT(KC_E)},               // E
    {.codepoint = 0x46, .keycode = LSFT(KC_F)},               // F
    {.codepoint = 0x47, .keycode = LSFT(KC_G)},               // G
    {.codepoint = 0x48, .keycode = LSFT(KC_H)},               // H
    {.codepoint = 0x49, .keycode = LSFT(KC_I)},               // I
    {.codepoint = 0x4A, .keycode = LSFT(KC_J)},               // J
    {.codepoint = 0x4B, .keycode = LSFT(KC_K)},               // K
    {.codepoint = 0x4C, .keycode = LSFT(KC_L)},               // L
    {.codepoint = 0x4D, .keycode = LSFT(KC_M)},               // M
    {.codepoint = 0x4E, .keycode = LSFT(KC_N)},               // N
    {.codepoint = 0x4F, .keycode = LSFT(KC_O)},               // O
    {.codepoint = 0x50, .keycode = LSFT(KC_P)},               // P
    {.codepoint = 0x51, .keycode = LSFT(KC_Q)},               // Q
    {.codepoint = 0x52, .keycode = LSFT(KC_R)},               // R
    {.codepoint = 0x53, .keycode = LSFT(KC_S)},               // S
    {.codepoint = 0x54, .keycode = LSFT(KC_T)},               // T
    {.codepoint = 0x55, .keycode = LSFT(KC_U)},               // U
    {.codepoint = 0x56, .keycode = LSFT(KC_V)},               // V
    {.codepoint = 0x57, .keycode = LSFT(KC_W)},               // W
    {.codepoint = 0x58, .keycode = LSFT(KC_X)},               // X
    {.codepoint = 0x59, .keycode = LSFT(KC_Y)},               // Y
    {.codepoint = 0x5A, .keycode = LSFT(KC_Z)},               // Z
    {.codepoint = 0x5B, .keycode = KC_LBRACKET},              // [
    {.codepoint = 0x5C, .keycode = KC_BSLASH},                /* \ */
    {.codepoint = 0x5D, .keycode = KC_RBRACKET},              // ]
    {.codepoint = 0x5E, .keycode = KC_CIRCUMFLEX},            // ^
    {.codepoint = 0x5F, .keycode = KC_UNDERSCORE},            // _
    {.codepoint = 0x60, .keycode = KC_GRAVE},                 // `
    {.codepoint = 0x61, .keycode = KC_A},                     // a
    {.codepoint = 0x62, .keycode = KC_B},                     // b
    {.codepoint = 0x63, .keycode = KC_C},                     // c
    {.codepoint = 0x64, .keycode = KC_D},                     // d
    {.codepoint = 0x65, .keycode = KC_E},                     // e
    {.codepoint = 0x66, .keycode = KC_F},                     // f
    {.codepoint = 0x67, .keycode = KC_G},                     // g
    {.codepoint = 0x68, .keycode = KC_H},                     // h
    {.codepoint = 0x69, .keycode = KC_I},                     // i
    {.codepoint = 0x6A, .keycode = KC_J},                     // j
    {.codepoint = 0x6B, .keycode = KC_K},                     // k
    {.codepoint = 0x6C, .keycode = KC_L},                     // l
    {.codepoint = 0x6D, .keycode = KC_M},                     // m
    {.codepoint = 0x6E, .keycode = KC_N},                     // n
    {.codepoint = 0x6F, .keycode = KC_O},                     // o
    {.codepoint = 0x70, .keycode = KC_P},                     // p
    {.codepoint = 0x71, .keycode = KC_Q},                     // q
    {.codepoint = 0x72, .keycode = KC_R},                     // r
    {.codepoint = 0x73, .keycode = KC_S},                     // s
    {.codepoint = 0x74, .keycode = KC_T},                     // t
    {.codepoint = 0x75, .keycode = KC_U},                     // u
    {.codepoint = 0x76, .keycode = KC_V},                     // v
    {.codepoint = 0x77, .keycode = KC_W},                     // w
    {.codepoint = 0x78, .keycode = KC_X},                     // x
    {.codepoint = 0x79, .keycode = KC_Y},                     // y
    {.codepoint = 0x7A, .keycode = KC_Z},                     // z
    {.codepoint = 0x7B, .keycode = KC_LEFT_CURLY_BRACE},      // {
    {.codepoint = 0x7C, .keycode = KC_PIPE},                  // |
    {.codepoint = 0x7D, .keycode = KC_RIGHT_CURLY_BRACE},     // }
    {.codepoint = 0x7E, .keycode = KC_TILDE},                 // ~
    }
  };
#endif

#ifdef LOAD_C2K_FR_FR_AS
const c2k_table_t PROGMEM c2k_fr_fr = {
  .size = 97,
  .table = {
    {.codepoint = 0x9, .keycode = KC_TAB},                    // Horizontal Tab (\t)
    {.codepoint = 0xA, .keycode = KC_ENTER},                  // Line Feed (\n)
    {.codepoint = 0x20, .keycode = KC_SPACE},                 // Space
    {.codepoint = 0x21, .keycode = KC_SLASH},                 // !
    {.codepoint = 0x22, .keycode = KC_3},                     // "
    {.codepoint = 0x23, .keycode = RALT(KC_3)},               // #
    {.codepoint = 0x24, .keycode = KC_RBRACKET},              // $
    {.codepoint = 0x25, .keycode = LSFT(KC_QUOTE)},           // %
    {.codepoint = 0x26, .keycode = KC_1},                     // &
    {.codepoint = 0x27, .keycode = KC_4},                     // '
    {.codepoint = 0x28, .keycode = KC_5},                     // (
    {.codepoint = 0x29, .keycode = KC_MINUS},                 // )
    {.codepoint = 0x2A, .keycode = KC_BSLASH},                // *
    {.codepoint = 0x2B, .keycode = LSFT(KC_EQUAL)},           // +
    {.codepoint = 0x2C, .keycode = KC_M},                     // ,
    {.codepoint = 0x2D, .keycode = KC_6},                     // -
    {.codepoint = 0x2E, .keycode = LSFT(KC_COMMA)},           // .
    {.codepoint = 0x2F, .keycode = LSFT(KC_DOT)},             // /
    {.codepoint = 0x30, .keycode = LSFT(KC_0)},               // 0
    {.codepoint = 0x31, .keycode = LSFT(KC_1)},               // 1
    {.codepoint = 0x32, .keycode = LSFT(KC_2)},               // 2
    {.codepoint = 0x33, .keycode = LSFT(KC_3)},               // 3
    {.codepoint = 0x34, .keycode = LSFT(KC_4)},               // 4
    {.codepoint = 0x35, .keycode = LSFT(KC_5)},               // 5
    {.codepoint = 0x36, .keycode = LSFT(KC_6)},               // 6
    {.codepoint = 0x37, .keycode = LSFT(KC_7)},               // 7
    {.codepoint = 0x38, .keycode = LSFT(KC_8)},               // 8
    {.codepoint = 0x39, .keycode = LSFT(KC_9)},               // 9
    {.codepoint = 0x3A, .keycode = KC_DOT},                   // :
    {.codepoint = 0x3B, .keycode = KC_COMMA},                 // ;
    {.codepoint = 0x3C, .keycode = KC_LEFT_ANGLE_BRACKET},    // <
    {.codepoint = 0x3D, .keycode = KC_EQUAL},                 // =
    {.codepoint = 0x3E, .keycode = LSFT(KC_LEFT_ANGLE_BRACKET)},   // >
    {.codepoint = 0x3F, .keycode = LSFT(KC_M)},               // ?
    {.codepoint = 0x40, .keycode = RALT(KC_0)},               // @
    {.codepoint = 0x41, .keycode = LSFT(KC_Q)},               // A
    {.codepoint = 0x42, .keycode = LSFT(KC_B)},               // B
    {.codepoint = 0x43, .keycode = LSFT(KC_C)},               // C
    {.codepoint = 0x44, .keycode = LSFT(KC_D)},               // D
    {.codepoint = 0x45, .keycode = LSFT(KC_E)},               // E
    {.codepoint = 0x46, .keycode = LSFT(KC_F)},               // F
    {.codepoint = 0x47, .keycode = LSFT(KC_G)},               // G
    {.codepoint = 0x48, .keycode = LSFT(KC_H)},               // H
    {.codepoint = 0x49, .keycode = LSFT(KC_I)},               // I
    {.codepoint = 0x4A, .keycode = LSFT(KC_J)},               // J
    {.codepoint = 0x4B, .keycode = LSFT(KC_K)},               // K
    {.codepoint = 0x4C, .keycode = LSFT(KC_L)},               // L
    {.codepoint = 0x4D, .keycode = LSFT(KC_SCOLON)},          // M
    {.codepoint = 0x4E, .keycode = LSFT(KC_N)},               // N
    {.codepoint = 0x4F, .keycode = LSFT(KC_O)},               // O
    {.codepoint = 0x50, .keycode = LSFT(KC_P)},               // P
    {.codepoint = 0x51, .keycode = LSFT(KC_A)},               // Q
    {.codepoint = 0x52, .keycode = LSFT(KC_R)},               // R
    {.codepoint = 0x53, .keycode = LSFT(KC_S)},               // S
    {.codepoint = 0x54, .keycode = LSFT(KC_T)},               // T
    {.codepoint = 0x55, .keycode = LSFT(KC_U)},               // U
    {.codepoint = 0x56, .keycode = LSFT(KC_V)},               // V
    {.codepoint = 0x57, .keycode = LSFT(KC_Z)},               // W
    {.codepoint = 0x58, .keycode = LSFT(KC_X)},               // X
    {.codepoint = 0x59, .keycode = LSFT(KC_Y)},               // Y
    {.codepoint = 0x5A, .keycode = LSFT(KC_W)},               // Z
    {.codepoint = 0x5B, .keycode = RALT(KC_5)},               // [
    {.codepoint = 0x5C, .keycode = RALT(KC_8)},               /* \ */
    {.codepoint = 0x5D, .keycode = RALT(KC_MINUS)},           // ]
    {.codepoint = 0x5E, .keycode = KC_LBRACKET},              // ^
    {.codepoint = 0x5F, .keycode = KC_8},                     // _
    {.codepoint = 0x60, .keycode = RALT(KC_7)},               // `
    {.codepoint = 0x61, .keycode = KC_Q},                     // a
    {.codepoint = 0x62, .keycode = KC_B},                     // b
    {.codepoint = 0x63, .keycode = KC_C},                     // c
    {.codepoint = 0x64, .keycode = KC_D},                     // d
    {.codepoint = 0x65, .keycode = KC_E},                     // e
    {.codepoint = 0x66, .keycode = KC_F},                     // f
    {.codepoint = 0x67, .keycode = KC_G},                     // g
    {.codepoint = 0x68, .keycode = KC_H},                     // h
    {.codepoint = 0x69, .keycode = KC_I},                     // i
    {.codepoint = 0x6A, .keycode = KC_J},                     // j
    {.codepoint = 0x6B, .keycode = KC_K},                     // k
    {.codepoint = 0x6C, .keycode = KC_L},                     // l
    {.codepoint = 0x6D, .keycode = KC_SCOLON},                // m
    {.codepoint = 0x6E, .keycode = KC_N},                     // n
    {.codepoint = 0x6F, .keycode = KC_O},                     // o
    {.codepoint = 0x70, .keycode = KC_P},                     // p
    {.codepoint = 0x71, .keycode = KC_A},                     // q
    {.codepoint = 0x72, .keycode = KC_R},                     // r
    {.codepoint = 0x73, .keycode = KC_S},                     // s
    {.codepoint = 0x74, .keycode = KC_T},                     // t
    {.codepoint = 0x75, .keycode = KC_U},                     // u
    {.codepoint = 0x76, .keycode = KC_V},                     // v
    {.codepoint = 0x77, .keycode = KC_Z},                     // w
    {.codepoint = 0x78, .keycode = KC_X},                     // x
    {.codepoint = 0x79, .keycode = KC_Y},                     // y
    {.codepoint = 0x7A, .keycode = KC_W},                     // z
    {.codepoint = 0x7B, .keycode = RALT(KC_4)},               // {
    {.codepoint = 0x7C, .keycode = RALT(KC_6)},               // |
    {.codepoint = 0x7D, .keycode = RALT(KC_EQUAL)},           // }
    {.codepoint = 0x7E, .keycode = RALT(KC_2)},               // ~
    }
  };
#endif

const c2k_table_t* c2k_tables[] = {
  #ifdef LOAD_C2K_EN_US_AS
  [LOAD_C2K_EN_US_AS] = &c2k_en_us,
  #endif
  #ifdef LOAD_C2K_FR_FR_AS
  [LOAD_C2K_FR_FR_AS] = &c2k_fr_fr,
  #endif
};

#endif
