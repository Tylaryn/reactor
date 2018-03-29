#include "ergodox_ez.h"
#include "debug.h"
#include "action_layer.h"
#include "version.h"
#include "utf8_decoder.h"

#define LOAD_C2K_EN_US_AS 0
#define LOAD_C2K_FR_FR_AS 1
#include "char_to_keycode.h"

#include "custom_modified_values.h"

#define SEND_STRING_CUSTOM(str) utf8_walk_characters(str, send_utf8_string)



#define __________ KC_TRANSPARENT
#define OS_LNX 0
#define OS_WIN 1
#define OS_OSX 2

bool is_azerty;
uint8_t mods;
uint8_t os_host;

/* LAYERS */
enum {
  _COLEMAK = 0,
  _SYMBOLS,
  _NAVIGAT,
  _QWERTY,
  _AZERTY,
};

/* MACROS */
enum {
  TO_FRENCH = 1,
  TO_ENGLISH,
  CUR_STATE,
};

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD,

};

const uint16_t PROGMEM keymaps[]/*[LOADED_C2K_TABLES]*/[MATRIX_ROWS][MATRIX_COLS] = {

  [_COLEMAK] = KEYMAP(
       UC(0x3D),     UC(0x31),   UC(0x32),   UC(0x33),   UC(0x34),   UC(0x35),   KC_LEFT,
      KC_DELETE,     UC(0x71),   UC(0x77),   UC(0x65),   UC(0x72),   UC(0x74),     TG(1),
      KC_BSPACE,     UC(0x61),   UC(0x73),   UC(0x64),   UC(0x66),   UC(0x67),
      KC_LSHIFT,     UC(0x7A),   UC(0x78),   UC(0x63),   UC(0x76),   UC(0x62),ALL_T(KC_NO),
   M(CUR_STATE),     KC_QUOTE,LALT(KC_LSFT),  KC_LEFT,   KC_LCTRL,
                                                                       KC_APP,    KC_LGUI,
                                                                                  KC_PGDN,
                                                         UC(0x20),   UC(0x09),     KC_END,

       KC_RIGHT,     UC(0x36),   UC(0x37),   UC(0x38),   UC(0x39),   UC(0x30),   UC(0x2D),
          TG(2),     UC(0x79),   UC(0x75),   UC(0x69),   UC(0x6F),   UC(0x70),   UC(0x5C),
                     UC(0x68),   UC(0x6A),   UC(0x6B),   UC(0x6C),   UC(0x3B),   UC(0x27),
    MEH_T(KC_NO),    UC(0x6E),   UC(0x6D),   UC(0x2C),   UC(0x2E),   UC(0x2F),  KC_RSHIFT,
                                    KC_UP,    KC_DOWN,   UC(0x5B),   UC(0x5D),      MO(1),

         KC_RALT,      KC_ESC,
         KC_PGUP,
         KC_HOME,    UC(0x0A),  KC_BSPACE
  ),

  [_SYMBOLS] = KEYMAP(
        KC_ESCAPE,      KC_F1,      KC_F2,      KC_F3,      KC_F4,      KC_F5,    KC_TRNS,
          KC_TRNS,    KC_EXLM,      KC_AT,    KC_LCBR,    KC_RCBR,    KC_PIPE,    KC_TRNS,
          KC_TRNS,    KC_HASH,     KC_DLR,    KC_LPRN,    KC_RPRN,   KC_GRAVE,
          KC_TRNS,    KC_PERC,    KC_CIRC,KC_LBRACKET,KC_RBRACKET,    KC_TILD,    KC_TRNS,
          KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,    KC_TRNS,
                                                                      RGB_MOD,    KC_TRNS,
                                                                                  KC_TRNS,
                                                          RGB_VAD,    RGB_VAI,    KC_TRNS,

          KC_TRNS,      KC_F6,      KC_F7,      KC_F8,      KC_F9,      KC_F10,     KC_F11,
          KC_TRNS,      KC_UP,       KC_7,       KC_8,    KC_KP_9,     KC_ASTR,     KC_F12,
                      KC_DOWN,       KC_4,       KC_5,       KC_6,     KC_PLUS,    KC_TRNS,
          KC_TRNS,    KC_AMPR,       KC_1,       KC_2,       KC_3,   KC_BSLASH,    KC_TRNS,
                                  KC_TRNS,     KC_DOT,       KC_0,    KC_EQUAL,    KC_TRNS,
          RGB_TOG,    RGB_SLD,
          KC_TRNS,
          KC_TRNS,    RGB_HUD,    RGB_HUI
  ),

  [2] = KEYMAP(M(TO_FRENCH),M(TO_ENGLISH),M(CUR_STATE),KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_UP,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_LEFT,KC_MS_DOWN,KC_MS_RIGHT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MS_BTN1,KC_MS_BTN2,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MEDIA_PLAY_PAUSE,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_MEDIA_PREV_TRACK,KC_MEDIA_NEXT_TRACK,KC_TRANSPARENT,KC_TRANSPARENT,KC_AUDIO_VOL_UP,KC_AUDIO_VOL_DOWN,KC_AUDIO_MUTE,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_TRANSPARENT,KC_WWW_BACK),

  /*
  [LAYER] = KEYMAP(
    __________, __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________,
                                                                __________, __________,
                                                                            __________,
                                                    __________, __________, __________,

    __________, __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________, __________, __________,
                __________, __________, __________, __________, __________, __________,
    __________, __________, __________, __________, __________, __________, __________,
                            __________, __________, __________, __________, __________,
    __________, __________,
    __________,
    __________, __________, __________
  ),
  */
};

const uint16_t PROGMEM fn_actions[] = {
  [1] = ACTION_LAYER_TAP_TOGGLE(1)
};

// typedef struct {
//   uint16_t kc;
//   uint8_t row;
//   uint8_t col;
//   uint8_t layer;
//   bool is_pressed;
// } cached_key_t;

// cached_key_t cached_key = {.row = 255, .col = 255};

// struct mod_states {
//   bool lsft :1;
//   bool rsft :1;
//   bool ralt :1;
//   uint8_t other:5;
// };

// struct mod_states mods_state = {
//   .lsft = false,
//   .rsft = false,
//   .ralt = false,
//   .other = 0
// };

// struct {
//   bool is_held  :1;
//   uint8_t index :2;
//   uint8_t row;
//   uint8_t col;
//   uint16_t key[4];
// } held_key;


// void set_current_key(uint16_t default_kc, uint16_t shifted_kc, uint16_t altgred_kc, uint16_t sft_ralt_kc) {
//   held_key.key[0] = default_kc;
//   held_key.key[1] = shifted_kc;
//   held_key.key[2] = altgred_kc;
//   held_key.key[3] = sft_ralt_kc;
// }



// void remove_modifiers(bool lsft, bool rsft, bool ralt) {
//   if(lsft && mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
//   if(rsft && mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
//   if(ralt && mods_state.ralt) del_mods(MOD_BIT(KC_RALT));
// }



// void add_modifiers(bool lsft, bool rsft, bool ralt) {
//   if(lsft && mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
//   if(rsft && mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
//   if(ralt && mods_state.ralt) add_mods(MOD_BIT(KC_RALT));
// }

// uint16_t translate_keycode(uint16_t keycode) {
//   if(keycode < QK_UNICODE) return keycode;
//   uint16_t keycode_from_cp = get_keycode_from_codepoint(keycode & 0x7FFF);
//   if(keycode_from_cp == KC_NO) {
//     return keycode;
//   }
//   return keycode_from_cp;
// }



// uint16_t get_keycode_on_modifier_press(uint16_t mod_kc) {
//   // keyrecord_t* current_record = get_current_record();
//   if(mods_state.other) {
//     if(!IS_CHAR_MOD(mod_kc)) {
//       if(held_key.is_held) {
//         add_modifiers(true,true,true);
//         add_mods(MOD_BIT(mod_kc));
//         return held_key.key[0];
//       }
//     }
//   }
//   else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
//     if(held_key.is_held) {
//       if(held_key.key[3]) {
//         remove_modifiers(true,true,true);
//         return held_key.key[3];
//       }
//       add_modifiers(true,true,true);
//       return held_key.key[0];
//     }
//   }
//   else if(mods_state.lsft || mods_state.rsft) {
//     if(held_key.is_held) {
//       if(held_key.key[1]) {
//         remove_modifiers(true,true,false);
//         return held_key.key[1];
//       }
//       add_modifiers(true,true,false);
//       return held_key.key[0];
//     }
//   }
//   else if(mods_state.ralt) {
//     if(held_key.is_held) {
//       if(held_key.key[2]) {
//         remove_modifiers(false,false,true);
//         return held_key.key[2];
//       }
//       add_modifiers(false,false,true);
//       return held_key.key[0];
//     }
//   }
//   return mod_kc;
// }



// uint16_t get_keycode_on_modifier_release(uint16_t mod_kc) {
//   keyrecord_t* current_record = get_current_record();
//   if(mods_state.other) {
//     if(held_key.is_held && held_key.index) {
//       push_key_to_q((key2register_t){
//         .key = {
//           .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//           .pressed = false,
//           .time = 0
//         },
//         .kc = translate_keycode(held_key.key[held_key.index])
//       });
//       held_key.index = 0;
//       push_key_to_q((key2register_t){
//         .key = {
//           .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//           .pressed = true,
//           .time = 0
//         },
//         .kc = translate_keycode(held_key.key[0])
//       });
//     }
//   }
//   else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
//     if(held_key.is_held) {
//       current_record->event.pressed = true;
//       current_record->event.key.row = held_key.row;
//       current_record->event.key.col = held_key.col;
//       if(held_key.key[3]) {
//         return held_key.key[3];
//       }
//       add_modifiers(true,true,true);
//       return held_key.key[0];
//     }
//   }
//   else if(mods_state.lsft || mods_state.rsft) {
//     if(held_key.is_held) {
//       if(held_key.key[1]) {
//         push_key_to_q((key2register_t){
//           .key = {
//             .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//             .pressed = false,
//             .time = 0
//           },
//           .kc = translate_keycode(held_key.key[held_key.index])
//         });
//         held_key.index = 1;
//         push_key_to_q((key2register_t){
//           .key = {
//             .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//             .pressed = true,
//             .time = 0
//           },
//           .kc = translate_keycode(held_key.key[1])
//         });
//       } else {
//         add_modifiers(true,true,false);
//         return held_key.key[0];
//       }
//     }
//   }
//   else if(mods_state.ralt) {
//     if(held_key.is_held) {
//       current_record->event.pressed = true;
//       current_record->event.key.row = held_key.row;
//       current_record->event.key.col = held_key.col;
//       if(held_key.key[2]) {
//         return held_key.key[2];
//       }
//       add_modifiers(false,false,true);
//       return held_key.key[0];
//     }
//   } else {
//     if(held_key.is_held && held_key.index) {
//       push_key_to_q((key2register_t){
//         .key = {
//           .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//           .pressed = false,
//           .time = 0
//         },
//         .kc = translate_keycode(held_key.key[held_key.index])
//       });
//       held_key.index = 0;
//       push_key_to_q((key2register_t){
//         .key = {
//           .key = (keypos_t){ .row = held_key.row, .col = held_key.col },
//           .pressed = true,
//           .time = 0
//         },
//         .kc = translate_keycode(held_key.key[0])
//       });
//     }
//   }
//   return mod_kc;
// }



// uint16_t get_keycode_on_key_press(void) {
//   // keyrecord_t* current_record = get_current_record();
//   if(mods_state.other) {
//     // Return non modified key
//   }
//   else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
//     if(held_key.key[3]) {
//       remove_modifiers(true,true,true);
//       held_key.index = 3;
//       return held_key.key[3];
//     }
//   }
//   else if(mods_state.lsft || mods_state.rsft) {
//     if(held_key.key[1]) {
//       remove_modifiers(true,true,false);
//       held_key.index = 1;
//       return held_key.key[1];
//     }
//   }
//   else if(mods_state.ralt) {
//     if(held_key.key[2]) {
//       remove_modifiers(false,false,true);
//       held_key.index = 2;
//       return held_key.key[2];
//     }
//   }
//   held_key.index = 0;
//   return held_key.key[0];
// }

// // KEYBOARD_REPORT_KEYS is used to know how many keys can be pressed simultaneousely

// uint16_t get_keycode_on_key_release(void) {
//   // keyrecord_t* current_record = get_current_record();
//   if(mods_state.other) {
//     // Return non modified key
//   }
//   else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
//     if(held_key.key[3]) {
//       add_modifiers(true,true,true);
//       return held_key.key[3];
//     }
//   }
//   else if(mods_state.lsft || mods_state.rsft) {
//     // We get here for some reason after releasing = after shift was released
//     if(held_key.key[1]) {
//       add_modifiers(true,true,false);
//       return held_key.key[1];
//     }
//   }
//   else if(mods_state.ralt) {
//     if(held_key.key[2]) {
//       add_modifiers(false,false,true);
//       return held_key.key[2];
//     }
//   }
//   return held_key.key[0];
// }



// uint16_t get_keycode_depending_on_modifiers(uint16_t mod_kc) {
//   keyrecord_t* current_record = get_current_record();
//   if(mods_state.other) {
//     if(mod_kc) {
//       if(held_key.is_held) {
//         return held_key.key[0];
//       }
//       return mod_kc;
//     }
//     return held_key.key[0];
//   }
//   else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
//     if(held_key.is_held && held_key.key[3]) {
//       if(mod_kc && !current_record->event.pressed) {
//         current_record->event.pressed = true;
//         return held_key.key[3];
//       }
//       if(mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
//       if(mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
//       if(mods_state.ralt) del_mods(MOD_BIT(KC_RALT));
//       return held_key.key[3];
//     }
//     if(mod_kc) {
//       return mod_kc;
//     }
//     if(mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
//     if(mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
//     if(mods_state.ralt) add_mods(MOD_BIT(KC_RALT));
//     if(!current_record->event.pressed) return held_key.key[3] ? held_key.key[3] : held_key.key[0];
//   }
//   else if(mods_state.lsft || mods_state.rsft) {
//     if(held_key.is_held && held_key.key[1]) {
//       if(mod_kc && !current_record->event.pressed) {
//         current_record->event.pressed = true;
//         return held_key.key[1];
//       }
//       if(mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
//       if(mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
//       return held_key.key[1];
//     }
//     if(mod_kc) {
//       return mod_kc;
//     }
//     if(mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
//     if(mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
//     if(!current_record->event.pressed) return held_key.key[1] ? held_key.key[1] : held_key.key[0];
//   }
//   else if(mods_state.ralt) {
//     if(held_key.is_held && held_key.key[2]) {
//       if(mod_kc && !current_record->event.pressed) {
//         current_record->event.pressed = true;
//         return held_key.key[2];
//       }
//       del_mods(MOD_BIT(KC_RALT));
//       return held_key.key[2];
//     }
//     if(mod_kc) {
//       return mod_kc;
//     }
//     add_mods(MOD_BIT(KC_RALT));
//     if(!current_record->event.pressed) return held_key.key[2] ? held_key.key[2] : held_key.key[0];
//   } else {
//     if(mod_kc) {
//       if(held_key.is_held) {
//         add_key(held_key.key[0]);
//       }
//       return mod_kc;
//     }
//   }
//   return held_key.key[0];
// }

// uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
//   if(get_key_q()->c >= 0) {
//     return get_key_q()->q[get_key_q()->c].kc;
//   }
//   keyrecord_t* current_record = get_current_record();
//   if(current_record->event.pressed == cached_key.is_pressed && layer == cached_key.layer && key.row == cached_key.row && key.col == cached_key.col) return cached_key.kc;
//   cached_key.is_pressed = current_record->event.pressed;
//   cached_key.layer = layer;
//   cached_key.row = key.row;
//   cached_key.col = key.col;
//   cached_key.kc = pgm_read_word(&keymaps[(layer)][(key.row)][(key.col)]);
//   if(IS_MODIFIER(cached_key.kc)) {
//     if(current_record->event.pressed) {
//       if(cached_key.kc == KC_LSFT) mods_state.lsft = true;
//       else if(cached_key.kc == KC_RSFT) mods_state.rsft = true;
//       else if(cached_key.kc == KC_RALT) mods_state.ralt = true;
//       else {
//         switch(cached_key.kc) {
//           case KC_LCTRL:
//             mods_state.other |= 0b10000;
//           break;
//           case KC_LALT:
//             mods_state.other |= 0b01000;
//           break;
//           case KC_LGUI:
//             mods_state.other |= 0b00100;
//           break;
//           case KC_RCTRL:
//             mods_state.other |= 0b00010;
//           break;
//           case KC_RGUI:
//             mods_state.other |= 0b00001;
//           break;
//         }
//       }
//       cached_key.kc = get_keycode_on_modifier_press(cached_key.kc);
//     } else {
//       if(cached_key.kc == KC_LSFT) mods_state.lsft = false;
//       else if(cached_key.kc == KC_RSFT) mods_state.rsft = false;
//       else if(cached_key.kc == KC_RALT) mods_state.ralt = false;
//       else {
//         switch(cached_key.kc) {
//           case KC_LCTRL:
//             mods_state.other &= 0b01111;
//           break;
//           case KC_LALT:
//             mods_state.other &= 0b10111;
//           break;
//           case KC_LGUI:
//             mods_state.other &= 0b11011;
//           break;
//           case KC_RCTRL:
//             mods_state.other &= 0b11101;
//           break;
//           case KC_RGUI:
//             mods_state.other &= 0b11110;
//           break;
//         }
//       }
//       cached_key.kc = get_keycode_on_modifier_release(cached_key.kc);
//       cached_key.is_pressed = current_record->event.pressed;
//       cached_key.row = current_record->event.key.row;
//       cached_key.col = current_record->event.key.col;
//     }
//   } else {
//     held_key.is_held = current_record->event.pressed;
//     held_key.row = key.row;
//     held_key.col = key.col;
//     switch(layer) {
//       case _COLEMAK:
//         switch(key.row) {
//           case 0:
//             switch(key.col) {
//               case 0:
//                 set_current_key(cached_key.kc, UC(0x27), 0, UC(0x39));
//               break;
//               default:
//                 set_current_key(cached_key.kc, 0, 0, 0);
//             }
//           break;
//           default:
//             set_current_key(cached_key.kc, 0, 0, 0);
//         }
//       break;
//       default:
//         set_current_key(cached_key.kc, 0, 0, 0);
//     }
//     if(held_key.is_held) cached_key.kc = get_keycode_on_key_press();
//     else cached_key.kc = get_keycode_on_key_release();
//     // cached_key.is_pressed = current_record->event.pressed;
//     cached_key.kc = translate_keycode(cached_key.kc);
//   }
//   return cached_key.kc;
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {

// }

// struct char_cache {
//   uint16_t kc       :16;
//   uint8_t row       :8;
//   uint8_t col       :8;
//   uint8_t layer     :8;
//   bool reg_lsft     :1;
//   bool reg_rsft     :1;
//   bool reg_ralt     :1;
// };


// struct char_cache cached_key = {.row = 255, .col = 255};

// uint16_t custom_modified_key(uint16_t default_kc, uint32_t shifted_cp, uint32_t altgred_cp, uint32_t shft_algr_cp) {
//   if((mods_state.lsft || mods_state.rsft) && mods_state.ralt && shft_algr_cp != 0) {
//     if(mods_state.lsft) {
//       cached_key.reg_lsft = true;
//     }
//     if(mods_state.rsft) {
//       cached_key.reg_rsft = true;
//     }
//     cached_key.reg_ralt = true;
//     return get_keycode_from_codepoint(shft_algr_cp);
//   }

//   if((mods_state.lsft || mods_state.rsft) && shifted_cp != 0) {
//     if(mods_state.lsft) {
//       cached_key.reg_lsft = true;
//     }
//     if(mods_state.rsft) {
//       cached_key.reg_rsft = true;
//     }
//     return get_keycode_from_codepoint(shifted_cp);
//   }

//   if(mods_state.ralt && altgred_cp != 0) {
//     cached_key.reg_ralt = true;
//     return get_keycode_from_codepoint(altgred_cp);
//   }

//   return default_kc;
// }

// uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
//   if(
//     layer == cached_key.layer &&
//     key.row == cached_key.row &&
//     key.col == cached_key.col
//   ) {
//     return cached_key.kc;
//   }
//   cached_key.layer = layer;
//   cached_key.row = key.row;
//   cached_key.col = key.col;

//   cached_key.kc = pgm_read_word(&keymaps[(layer)][(key.row)][(key.col)]);
//   if(cached_key.kc < QK_UNICODE) return cached_key.kc;
//   uint16_t keycode_from_cp = get_keycode_from_codepoint(cached_key.kc & 0x7FFF);
//   if(keycode_from_cp == KC_NO) {
//     return cached_key.kc;
//   }
//   cached_key.kc = keycode_from_cp;
//   if(!mods_state.lsft && !mods_state.rsft && !mods_state.ralt) return cached_key.kc;

//   switch(layer) {
//     case _COLEMAK:
//       switch(key.row) {
//         case 0:
//           switch(key.col) {
//             case 0:
//               cached_key.kc = custom_modified_key(cached_key.kc, 0x27, 0x29, 0x39);
//           }
//         break;
//       }
//     break;
//   }

//   return cached_key.kc;
// };

// void unregister_saved_mods(void) {
//   if(cached_key.reg_lsft) {
//     del_mods(MOD_BIT(KC_LSFT));
//   }
//   if(cached_key.reg_rsft) {
//     del_mods(MOD_BIT(KC_RSFT));
//   }
//   if(cached_key.reg_ralt) {
//     del_mods(MOD_BIT(KC_RALT));
//   }
// }

// void register_saved_mods(void) {
//   if(cached_key.reg_lsft) {
//     add_mods(MOD_BIT(KC_LSFT));
//     cached_key.reg_lsft = false;
//   }
//   if(cached_key.reg_rsft) {
//     add_mods(MOD_BIT(KC_RSFT));
//     cached_key.reg_rsft = false;
//   }
//   if(cached_key.reg_ralt) {
//     add_mods(MOD_BIT(KC_RALT));
//     cached_key.reg_ralt = false;
//   }
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch(keycode) {
//     case KC_LSFT:
//       mods_state.lsft = record->event.pressed;
//     break;
//     case KC_RSFT:
//       mods_state.rsft = record->event.pressed;
//     break;
//     case KC_RALT:
//       mods_state.ralt = record->event.pressed;
//     break;
//   }

//   keypos_t key = record->event.key;
//   switch(cached_key.layer) {
//     case _COLEMAK:
//       switch(key.row) {
//         case 0:
//           switch(key.col) {
//             case 0:
//               if(record->event.pressed) unregister_saved_mods();
//               else register_saved_mods();
//             break;
//           }
//         break;
//       }
//     break;
//   }
//   // switch (keycode) {
//   //   case EPRM:
//   //     if(record->event.pressed) custom_unicode_input(0xCA);
//   //     return false;
//   //   // Dont forget to return false
//   // }
//   return true;
// }

void keyboard_task_end_user(void) {
  // if((cur_keycode & QK_LSFT) != 0) {
  //   del_key(KC_LSFT);
  // }
  // if((cur_keycode & QK_RSFT) != 0) {
  //   del_key(KC_RSFT);
  // }
  // if((cur_keycode & QK_RALT) != 0) {
  //   del_key(KC_RALT);
  // }
}



void tap(uint16_t code) {
  register_code(code);
  unregister_code(code);
}

const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case CUR_STATE:
          SEND_STRING_CUSTOM("A#");
        break;

        case TO_FRENCH:
          set_cur_c2k_table(LOAD_C2K_FR_FR_AS);
        break;

        case TO_ENGLISH:
          set_cur_c2k_table(LOAD_C2K_EN_US_AS);
        break;

        case 0:
        if (record->event.pressed) {
          SEND_STRING_CUSTOM (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
      }
    return MACRO_NONE;
};



void matrix_init_user(void) {
  debug_enable=true;
  is_azerty = false;
  os_host = OS_LNX;
};



/** Start: Custom Unicode feature to be able to put Unicodes characters on modified keys (e.g. shifted keys) */

uint16_t hex_2_keycode(uint16_t hex) {
  // Replaced KC_* by KC_KP_* for french layout
  if (hex == 0x0) {
    return KC_KP_0;
  } else if (hex < 0xA) {
    return KC_KP_1 + (hex - 0x1);
  } else {
    switch(hex) {
      case 0xA: return is_azerty ? KC_Q : KC_A;
      case 0xB: return KC_B;
      case 0xC: return KC_C;
      case 0xD: return KC_D;
      case 0xE: return KC_E;
      case 0xF: return KC_F;
      default: return KC_NO;
    }
  }
}

void custom_unicode_input (uint16_t hex) {
  // save current mods
  mods = keyboard_report->mods;

  // unregister all mods to start from clean state
  if (mods & MOD_BIT(KC_LSFT)) unregister_code(KC_LSFT);
  if (mods & MOD_BIT(KC_RSFT)) unregister_code(KC_RSFT);
  if (mods & MOD_BIT(KC_LCTL)) unregister_code(KC_LCTL);
  if (mods & MOD_BIT(KC_RCTL)) unregister_code(KC_RCTL);
  if (mods & MOD_BIT(KC_LALT)) unregister_code(KC_LALT);
  if (mods & MOD_BIT(KC_RALT)) unregister_code(KC_RALT);
  if (mods & MOD_BIT(KC_LGUI)) unregister_code(KC_LGUI);
  if (mods & MOD_BIT(KC_RGUI)) unregister_code(KC_RGUI);

  switch(os_host) {
  case OS_LNX:
    register_code(KC_LCTL);
    register_code(KC_LSFT);
    register_code(KC_U);
    unregister_code(KC_U);
    unregister_code(KC_LSFT);
    unregister_code(KC_LCTL);
    break;
  case OS_WIN:
    register_code(KC_LALT);
    register_code(KC_PPLS);
    unregister_code(KC_PPLS);
    break;
  // case UC_WINC:
  //   register_code(KC_RALT);
  //   unregister_code(KC_RALT);
  //   register_code(KC_U);
  //   unregister_code(KC_U);
  //   break;
  case OS_OSX:
    register_code(KC_LALT);
    break;
  // case UC_OSX_RALT:
  //   register_code(KC_RALT);
  //   break;
  }

  for(int i = 3; i >= 0; i--) {
    uint16_t digit = ((hex >> (i*4)) & 0xF);
    if(digit != 0x0) tap(hex_2_keycode(digit));
  }
  // send_keyboard_report();

  switch(os_host) {
    case OS_OSX:
    case OS_WIN:
      unregister_code(KC_LALT);
      break;
    // case UC_OSX_RALT:
    //   unregister_code(KC_RALT);
    //   break;
    case OS_LNX:
      register_code(KC_SPC);
      unregister_code(KC_SPC);
      break;
  }

  // reregister previously set mods
  if (mods & MOD_BIT(KC_LSFT)) register_code(KC_LSFT);
  if (mods & MOD_BIT(KC_RSFT)) register_code(KC_RSFT);
  if (mods & MOD_BIT(KC_LCTL)) register_code(KC_LCTL);
  if (mods & MOD_BIT(KC_RCTL)) register_code(KC_RCTL);
  if (mods & MOD_BIT(KC_LALT)) register_code(KC_LALT);
  if (mods & MOD_BIT(KC_RALT)) register_code(KC_RALT);
  if (mods & MOD_BIT(KC_LGUI)) register_code(KC_LGUI);
  if (mods & MOD_BIT(KC_RGUI)) register_code(KC_RGUI);
}

/** End: Custom Unicode feature to be able to put Unicodes characters on modified keys (e.g. shifted keys) */


uint32_t layer_state_set_user(uint32_t state) {

    uint8_t layer = biton32(state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();
    switch (layer) {
      case 0:
      break;
      case 1:
        ergodox_right_led_3_on();
        break;
      case 2:
        ergodox_right_led_2_on();
        break;
      case 3:
        ergodox_right_led_1_on();
        break;
      case 4:
        ergodox_right_led_3_on();
        ergodox_right_led_2_on();
        break;
      case 5:
        ergodox_right_led_3_on();
        ergodox_right_led_1_on();
        break;
      case 6:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        break;
      case 7:
        ergodox_right_led_1_on();
        ergodox_right_led_2_on();
        ergodox_right_led_3_on();
        break;
      default:
        break;
    }

    return state;
};
