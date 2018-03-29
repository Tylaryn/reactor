#ifndef _CUSTOM_MODIFIED_VALUES_H_
#define _CUSTOM_MODIFIED_VALUES_H_

#ifndef CUSTOM_MODIFIED_VALUES_ENABLE
#define CUSTOM_MODIFIED_VALUES_ENABLE
#endif
#ifndef NUMBER_OF_HELD_KEYS
#define NUMBER_OF_HELD_KEYS 6
#endif

#include <stdint.h>
#include <stdbool.h>
#include "action.h"
#include "action_util.h"
#include "keyboard.h"
#include "char_to_keycode.h"

#define IS_MODIFIER(kc) (kc >= KC_LCTRL && kc <= KC_RGUI)

typedef struct {
  uint16_t kc;
  uint8_t row;
  uint8_t col;
  uint8_t layer   :5;
  bool is_pressed :1;
} cached_key_t;

struct {
  bool lsft :1;
  bool rsft :1;
  bool ralt :1;
  uint8_t other:5;
} mods_state;

typedef struct {
  keypos_t k;
  uint8_t l :5;
  uint8_t i :2;
  uint16_t kc[4];
} heldkey_t;

struct heldkeys {
  int8_t i :4;
  heldkey_t all[NUMBER_OF_HELD_KEYS];
};

struct heldkeys held_keys = {.i = -1};
cached_key_t cached_key = {.row = 255, .col = 255};
uint16_t custom_modified_values_buffer[4] = {0,0,0,0};


bool add_held_key(heldkey_t new_key) {
  if(held_keys.i == NUMBER_OF_HELD_KEYS - 1) return false;
  held_keys.all[++(held_keys.i)] = new_key;
  return true;
}

int8_t remove_held_key(uint8_t id) {
  int8_t cur_active_id = held_keys.i;
  uint8_t i = 0;
  for(; i <= cur_active_id; i++) {
    if(i > id) {
      held_keys.all[i - 1] = held_keys.all[i];
    }
  }
  held_keys.i = i-2;
  return cur_active_id;
}

heldkey_t* get_active_held_key(void) {
  return &(held_keys.all[held_keys.i]);
}

int8_t get_held_key_id(uint8_t layer, uint8_t row, uint8_t col) {
  uint8_t i = 0;
  for(; i <= held_keys.i; i++) {
    heldkey_t key = held_keys.all[i];
    if(key.l == layer && key.k.row == row && key.k.col == col) {
      return i;
    }
  }
  return -1;
}


void set_custom_modified_values_buffer(uint16_t kc_default, uint16_t kc_shifted, uint16_t kc_altgred, uint16_t kc_sftralt) {
  custom_modified_values_buffer[0] = kc_default;
  custom_modified_values_buffer[1] = kc_shifted;
  custom_modified_values_buffer[2] = kc_altgred;
  custom_modified_values_buffer[3] = kc_sftralt;
}


__attribute__ ((weak))
void get_modified_array_from_key(uint16_t default_kc, uint8_t layer, uint8_t row, uint8_t col) {
  set_custom_modified_values_buffer(default_kc, 0, 0, 0);
  switch(layer) {
    case 0:
      switch(row) {
        case 0:
          switch(col) {
            case 0:
              // print("Layer 1 - Row 1 - Col 1.\n");
              set_custom_modified_values_buffer(default_kc, UC(0x27), 0, UC(0x41));
            break;
          }
        break;
      }
    break;
  }
}



void remove_modifiers(bool lsft, bool rsft, bool ralt) {
  if(lsft && mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
  if(rsft && mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
  if(ralt && mods_state.ralt) del_mods(MOD_BIT(KC_RALT));
}

void add_modifiers(bool lsft, bool rsft, bool ralt) {
  if(lsft && mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
  if(rsft && mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
  if(ralt && mods_state.ralt) add_mods(MOD_BIT(KC_RALT));
}

uint16_t translate_keycode(uint16_t keycode) {
  if(keycode < QK_UNICODE) return keycode;
  uint16_t keycode_from_cp = get_keycode_from_codepoint(keycode & 0x7FFF);
  if(keycode_from_cp == KC_NO) {
    return keycode;
  }
  return keycode_from_cp;
}

void add_to_q_release_and_press(heldkey_t* active_held_key, uint8_t new_kcid) {
  // print("add_to_q_release_and_press.\n");
  keyevent_describer_t ke_descr = {
    .row = active_held_key->k.row,
    .col = active_held_key->k.col,
    .p = false,
    .kc = translate_keycode(active_held_key->kc[active_held_key->i])
  };
  push_key_to_q(ke_descr);
  active_held_key->i = new_kcid;
  ke_descr.p = true;
  ke_descr.kc = translate_keycode(active_held_key->kc[new_kcid]);
  push_key_to_q(ke_descr);
}

uint16_t get_kc_on_modifier_event(uint16_t mod_kc) {
  if(held_keys.i == -1) return mod_kc;
  heldkey_t* active_held_key = get_active_held_key();
  if(mods_state.other) {
    add_modifiers(true,true,true);
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
    if(active_held_key->kc[3]) {
      remove_modifiers(true,true,true);
      add_to_q_release_and_press(active_held_key, 3);
      return KC_NO;
    }
    add_modifiers(true,true,true);
  } else if(mods_state.lsft || mods_state.rsft) {
    if(active_held_key->kc[1]) {
      remove_modifiers(true,true,false);
      add_to_q_release_and_press(active_held_key, 1);
      return KC_NO;
    }
    add_modifiers(true,true,false);
  } else if(mods_state.ralt) {
    if(active_held_key->kc[2]) {
      remove_modifiers(false,false,true);
      add_to_q_release_and_press(active_held_key, 2);
      return KC_NO;
    }
    add_modifiers(false,false,true);
  }
  if(active_held_key->i) {
    add_to_q_release_and_press(active_held_key, 0);
  }
  return mod_kc;
}

uint8_t get_kcid_on_key_press(void) {
  heldkey_t* active_held_key = get_active_held_key();
  if(mods_state.other) {
    return 0;
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt && active_held_key->kc[3]) {
    remove_modifiers(true,true,true);
    return 3;
  } else if((mods_state.lsft || mods_state.rsft) && active_held_key->kc[1]) {
    remove_modifiers(true,true,false);
    return 1;
  } else if(mods_state.ralt && active_held_key->kc[2]) {
    remove_modifiers(false,false,true);
    return 2;
  }
  return 0;
}

void fire_events_on_key_release(void) {
  if(mods_state.other) {
    add_modifiers(true,true,true);
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
    add_modifiers(true,true,true);
  } else if(mods_state.lsft || mods_state.rsft) {
    add_modifiers(true,true,false);
  } else if(mods_state.ralt) {
    add_modifiers(false,false,true);
  }
}

uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
  keyrecord_t* current_record = get_current_record();
  if(current_record->event.pressed == cached_key.is_pressed && layer == cached_key.layer && key.row == cached_key.row && key.col == cached_key.col) return cached_key.kc;
  cached_key.is_pressed = current_record->event.pressed;
  cached_key.layer = layer;
  cached_key.row = key.row;
  cached_key.col = key.col;
  cached_key.kc = pgm_read_word(&keymaps[(layer)][(key.row)][(key.col)]);
  if(IS_MODIFIER(cached_key.kc)) {
    if(current_record->event.pressed) {
      // print("Pressed a modifier.\n");
      if(cached_key.kc == KC_LSFT) mods_state.lsft = true;
      else if(cached_key.kc == KC_RSFT) mods_state.rsft = true;
      else if(cached_key.kc == KC_RALT) mods_state.ralt = true;
      else {
        switch(cached_key.kc) {
          case KC_LCTRL:
            mods_state.other |= 0b10000;
          break;
          case KC_LALT:
            mods_state.other |= 0b01000;
          break;
          case KC_LGUI:
            mods_state.other |= 0b00100;
          break;
          case KC_RCTRL:
            mods_state.other |= 0b00010;
          break;
          case KC_RGUI:
            mods_state.other |= 0b00001;
          break;
        }
      }
    } else { // If the modifier key is released
      // print("Released a modifier.\n");
      if(cached_key.kc == KC_LSFT) mods_state.lsft = false;
      else if(cached_key.kc == KC_RSFT) mods_state.rsft = false;
      else if(cached_key.kc == KC_RALT) mods_state.ralt = false;
      else {
        switch(cached_key.kc) {
          case KC_LCTRL:
            mods_state.other &= 0b01111;
          break;
          case KC_LALT:
            mods_state.other &= 0b10111;
          break;
          case KC_LGUI:
            mods_state.other &= 0b11011;
          break;
          case KC_RCTRL:
            mods_state.other &= 0b11101;
          break;
          case KC_RGUI:
            mods_state.other &= 0b11110;
          break;
        }
      }
    }
    cached_key.kc = get_kc_on_modifier_event(cached_key.kc);
  } else { // If a non modifier key has been pressed
    if(current_record->event.pressed) {
      // print("Pressed a non-modifier key.\n");
      get_modified_array_from_key(cached_key.kc, layer, key.row, key.col);
      add_held_key((heldkey_t){
        .k = (keypos_t){.row = key.row, .col = key.col},
        .l = layer,
        .kc = {custom_modified_values_buffer[0],custom_modified_values_buffer[1],custom_modified_values_buffer[2],custom_modified_values_buffer[3]}
      });
      heldkey_t* active_held_key = get_active_held_key();
      // if(active_held_key->kc[1]) print("Shifted value exists.\n");
      cached_key.kc = active_held_key->kc[(active_held_key->i = get_kcid_on_key_press())];
    } else { // If the key is released
      fire_events_on_key_release();
      if(get_key_q()->c == -1) { // If the key is NOT executed from the additional queue
        // print("Released a non-modifier key.\n");
        if(held_keys.i == -1) return cached_key.kc;
        int8_t released_held_key_id = get_held_key_id(layer, key.row, key.col);
        if(released_held_key_id == -1) return cached_key.kc;
        heldkey_t* released_held_key = &held_keys.all[released_held_key_id];
        cached_key.kc = released_held_key->kc[released_held_key->i];
        if(released_held_key_id == remove_held_key(released_held_key_id) && released_held_key_id) {
          released_held_key = get_active_held_key();
          uint8_t kcid = get_kcid_on_key_press();
          if(released_held_key->i != kcid) {
            add_to_q_release_and_press(released_held_key, kcid);
          }
        }
      } else { // If the key IS executed from the additional queue
        // print("Processing the releasing event of a non-modifier key executed from the 'additional keys' queue.\n");
        cached_key.kc = get_key_q()->q[get_key_q()->c].kc;
        remove_held_key(get_held_key_id(layer, key.row, key.col));
      }
    }
    cached_key.kc = translate_keycode(cached_key.kc);
  }
  return cached_key.kc;
}

#endif
