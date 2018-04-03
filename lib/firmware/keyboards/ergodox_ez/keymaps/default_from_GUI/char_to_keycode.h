#ifndef _CHAR_TO_KEYCODE_H_
#define _CHAR_TO_KEYCODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "char_to_keycode_tables.h"
#include "utf8_decoder.h"

enum types {
  TYPE_POINTER_TO_CHAR = 0,
  TYPE_UINT16,
  TYPE_OTHER,
};

#define GET_TYPE(var) _Generic((var), char *: TYPE_POINTER_TO_CHAR, short unsigned int: TYPE_UINT16, default: TYPE_OTHER)

uint8_t cur_c2k_table_index = 0;

__attribute__ ((weak))
void set_cur_c2k_table_user(uint8_t new_c2k_table_index) {
}

__attribute__ ((weak))
void set_cur_c2k_table_kb(uint8_t new_c2k_table_index) {
  set_cur_c2k_table_user(new_c2k_table_index);
}

void set_cur_c2k_table(uint8_t new_c2k_table_index) {
  cur_c2k_table_index = new_c2k_table_index;
  set_cur_c2k_table_kb(new_c2k_table_index);
}

uint16_t get_keycode_from_codepoint(const uint32_t codepoint) {
  uint32_t table_size = pgm_read_dword(&((*c2k_tables[cur_c2k_table_index]).size));
  for(uint32_t i = 0; i < table_size; i++) {
    if(pgm_read_dword(&((*c2k_tables[cur_c2k_table_index]).table[i].codepoint)) == codepoint) {
        return pgm_read_word(&((*c2k_tables[cur_c2k_table_index]).table[i].keycode));
    }
  }

  return KC_NO;
}

uint16_t translate_keycode(uint16_t keycode) {
  if(keycode < QK_UNICODE) return keycode;
  uint16_t keycode_from_cp = get_keycode_from_codepoint(keycode & 0x7FFF);
  if(keycode_from_cp == KC_NO) {
    return keycode;
  }
  return keycode_from_cp;
}

uint8_t send_utf8_string(uint32_t codepoint) {
  uint32_t keycode = get_keycode_from_codepoint(codepoint);
  register_code(keycode);
  unregister_code(keycode);
  return 0;
}

#endif
