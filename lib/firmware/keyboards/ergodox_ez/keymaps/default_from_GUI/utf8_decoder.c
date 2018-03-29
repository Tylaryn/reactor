#define UTF8_PREBUILT_FUNCTIONS
#include "utf8_decoder.h"

utf8_decoder_t utf8_decoder_init(const char* str) {
  utf8_decoder_t utf8_obj = (utf8_decoder_t) malloc(sizeof(utf8_decoder_struct_t));
  if(utf8_obj == NULL) {
    printf("Error: Failed to allocate memory for the utf8 decoder object.\n");
    return NULL;
  }
  utf8_obj->byte_c = 0;
  utf8_obj->char_c = 0;
  utf8_obj->status = UTF8_STATE_OK;
  utf8_obj->str = str;
  return utf8_obj;
}


void utf8_decoder_reset(utf8_decoder_t utf8_obj) {
  utf8_obj->byte_c = 0;
  utf8_obj->char_c = 0;
  utf8_obj->status = UTF8_STATE_OK;
}


void utf8_decoder_destroy(utf8_decoder_t utf8_obj) {
  free(utf8_obj);
}


int8_t utf8_decoder_status(utf8_decoder_t utf8_obj) {
  int8_t status = utf8_obj->status;
  if(status != UTF8_STATE_OK) {
    utf8_decoder_destroy(utf8_obj);
  }
  return status;
}


int8_t is_valid_codepoint(const uint32_t codepoint, const uint8_t number_of_bytes) {
  // printf("Codepoint: %d - Number of encoding bytes: %d\n", codepoint, number_of_bytes);
  if(codepoint > 0x10FFFF) {
    return UTF8_STATE_CODEPOINT_TOO_BIG;
  } else if(codepoint >= 0xD800 && codepoint <= 0xDFFF) { // Note: I believe WTF-8 standard doesn't consider this range as illegal, what should I do ?
    return UTF8_STATE_CODEPOINT_ILLEGAL_RANGE;
  } else if(codepoint <= 0x7F && number_of_bytes != 1 && codepoint != 0) { // Codepoints from U+0000 to U+007F (both included) must be encoded on 1 byte (except for NUL char)
    return UTF8_STATE_OVERLONG_ENCODING;
  } else if(codepoint >= 0x80 && codepoint <= 0x7FF && number_of_bytes != 2) { // Codepoints from U+0080 to U+07FF (both included) must be encoded on 2 bytes
    return UTF8_STATE_OVERLONG_ENCODING;
  } else if(codepoint >= 0x800 && codepoint <= 0xFFFF && number_of_bytes != 3) { // Codepoints from U+0800 to U+FFFF (both included) must be encoded on 3 bytes
    return UTF8_STATE_OVERLONG_ENCODING;
  } else if(codepoint >= 0x10000 && number_of_bytes != 4) { // Codepoints from U+10000 to U+10FFFF (both included) must be encoded on 4 bytes
    return UTF8_STATE_OVERLONG_ENCODING;
  }
  return UTF8_STATE_OK;
}


uint32_t utf8_next_character(utf8_decoder_t utf8_obj) {
  uint32_t codepoint = 0;
  uint8_t continuation_bytes = 0, byte_c = 1, bit_c;
  int8_t status;
  bool bit, read_rest;
  char cur_byte;
  process_byte:
  cur_byte = *(utf8_obj->str + utf8_obj->byte_c++);
  // printf("Byte #%d - Character: '%c'\n", utf8_obj->byte_c, cur_byte);
  if(cur_byte == 0) {
    if(continuation_bytes != 0) {
      utf8_obj->status = UTF8_STATE_UNEXPECTED_END_OF_STRING;
      return UTF8_INCORRECT;
    }
    utf8_obj->byte_c--;
    return 0x0;
  }
  read_rest = false;
  for(bit_c = 7; bit_c < 8; --bit_c) {
    bit = READ_BIT(cur_byte, bit_c);
    // printf("Bit #%d: %d\n", bit_c, bit);
    if(read_rest) {
      codepoint |= bit;
      if(bit_c != 0 || continuation_bytes != 0) {
        codepoint <<= 1;
      }
    } else if(bit_c == 7) {
      if(!bit && continuation_bytes != 0) {
        // printf("Missing expected continuation byte at bit #%d of byte #%d.\n", bit_c, utf8_obj->byte_c);
        utf8_obj->status = UTF8_STATE_MISSING_EXPECTED_CONTINUATION_BYTE;
        return UTF8_INCORRECT;
      }
      read_rest = !bit;
    } else if(bit_c == 6) {
      if(bit) {
        if(continuation_bytes > 0) {
          // printf("Missing expected continuation byte at bit #%d of byte #%d.\n", bit_c, utf8_obj->byte_c);
          utf8_obj->status = UTF8_STATE_MISSING_EXPECTED_CONTINUATION_BYTE;
          return UTF8_INCORRECT;
        }
        continuation_bytes = 1;
      } else {
        if(continuation_bytes == 0) {
          // printf("Unexpected continuation byte at bit #%d of byte #%d.\n", bit_c, utf8_obj->byte_c);
          utf8_obj->status = UTF8_STATE_UNEXPECTED_CONTINUATION_BYTE;
          return UTF8_INCORRECT;
        }
        continuation_bytes--;
        read_rest = true;
      }
    } else { // bit_c < 6
      if(bit) {
        if(bit_c < 4) {
          // printf("Illegal amount of encoding bytes at bit #%d of byte #%d.\n", bit_c, utf8_obj->byte_c);
          utf8_obj->status = UTF8_STATE_ILLEGAL_AMOUNT_OF_ENCODING_BYTES;
          return UTF8_INCORRECT;
        }
        continuation_bytes++;
      } else {
        byte_c = continuation_bytes + 1;
        read_rest = true;
      }
    }
  }

  if(continuation_bytes != 0) {
    goto process_byte;
  }
  if((status = is_valid_codepoint(codepoint, byte_c)) != UTF8_STATE_OK) {
    // printf("Invalid code point '%d' at byte #%d.\n", codepoint, utf8_obj->byte_c);
    utf8_obj->status = status;
    return UTF8_INCORRECT;
  }
  utf8_obj->char_c++;
  return codepoint;
}

#ifdef UTF8_PREBUILT_FUNCTIONS

#ifndef NO_UTF8_CHECK
int8_t utf8_check(const char* str) {
  utf8_decoder_t decoder = utf8_decoder_init(str);
  while(utf8_next_character(decoder) != 0) {
    if(decoder->status != UTF8_STATE_OK) {
      int8_t status = decoder->status;
      utf8_decoder_destroy(decoder);
      return status;
    }
  }
  int8_t status = decoder->status;
  utf8_decoder_destroy(decoder);
  return status;
}
#endif


#if !defined NO_UTF8_CHECK_OBJ || !defined NO_UTF8_STR_LENGTH || !defined NO_UTF8_TO_CODEPOINTS
int8_t utf8_check_obj(utf8_decoder_t decoder) {
  while(utf8_next_character(decoder) != 0) {
    if(decoder->status != UTF8_STATE_OK) return decoder->status;
  }
  return decoder->status;
}
#endif


#ifndef NO_UTF8_STR_LENGTH
int64_t utf8_str_length(const char* str) {
  utf8_decoder_t decoder = utf8_decoder_init(str);
  if(utf8_check_obj(decoder) != UTF8_STATE_OK) {
    printf("Error: String is not a correct UTF-8 string.\n");
    int8_t status = decoder->status;
    utf8_decoder_destroy(decoder);
    return status;
  }
  int64_t length = decoder->char_c;
  utf8_decoder_destroy(decoder);
  return length;
}
#endif


#ifndef NO_UTF8_TO_CODEPOINTS
uint32_t* utf8_to_codepoints(const char* str) {
  utf8_decoder_t decoder = utf8_decoder_init(str);
  if(utf8_check_obj(decoder) != UTF8_STATE_OK) {
    printf("Error: String is not a correct UTF-8 string.\n");
    utf8_decoder_destroy(decoder);
    return NULL;
  }
  utf8_decoder_reset(decoder);
  uint32_t* codepoints = (uint32_t *) calloc(decoder->char_c, sizeof(uint32_t));
  if(codepoints == NULL) {
    printf("Error: Failed to allocate memory for the codepoints array.\n");
    utf8_decoder_destroy(decoder);
    return NULL;
  }
  uint32_t codepoint;
  for(uint32_t i = 0; (codepoint = utf8_next_character(decoder)) != 0; i++) {
    if(decoder->status != U8_S_OK) {
      utf8_decoder_destroy(decoder);
      free(codepoints);
      return NULL;
    }
    *(codepoints + i) = codepoint;
  }
  if(decoder->status != U8_S_OK) {
    utf8_decoder_destroy(decoder);
    free(codepoints);
    return NULL;
  }
  utf8_decoder_destroy(decoder);
  return codepoints;
}
#endif


#ifndef NO_UTF8_WALK_CHARACTERS
int8_t utf8_walk_characters(const char* str, uint8_t (*process_codepoint)(uint32_t)) {
  utf8_decoder_t decoder = utf8_decoder_init(str);
  uint32_t codepoint;
  while((codepoint = utf8_next_character(decoder)) != 0) {
    if(decoder->status != UTF8_STATE_OK) {
      printf("Error: String is not a correct UTF-8 string.\n");
      int8_t status = decoder->status;
      utf8_decoder_destroy(decoder);
      return status;
    }
    process_codepoint(codepoint);
  }
  int8_t status = decoder->status;
  if(status != UTF8_STATE_OK) {
    printf("Error: String is not a correct UTF-8 string.\n");
  }
  utf8_decoder_destroy(decoder);
  return status;
}
#endif

#endif
