#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "utf8.h"

typedef struct {
  char* json_input;
  uint32_t bytes_read;
  uint32_t chars_read;
  uint16_t cur_char;
  uint8_t cur_state;
  uint8_t state_stack[];
} JSON_parser_t;

void json_parse(char* json_input) {
  JSON_parser_t json_parser;
  json_parser.json_input = json_input;
  utf8_to_codepoints(json_parser.json_input, );
}

#endif
