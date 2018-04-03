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
#define CHAR_MODS_SET 0b01100010

typedef struct {
  uint16_t kc;
  uint8_t row;
  uint8_t col;
  uint8_t layer   :5;
  bool transp_res :1;
  bool is_pressed :1;
} cached_key_t;

struct {
  bool lsft :1;
  bool rsft :1;
  bool ralt :1;
  bool other:1;
} mods_state;

typedef struct {
  keypos_t k;
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
  // print("Removing a held key from the array.\n");
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

int8_t get_held_key_id(uint8_t row, uint8_t col) {
  uint8_t i = 0;
  for(; i <= held_keys.i; i++) {
    heldkey_t key = held_keys.all[i];
    if(key.k.row == row && key.k.col == col) {
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



void remove_modifiers(bool sft, bool ralt) {
  if(sft && mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
  if(sft && mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
  if(ralt && mods_state.ralt) del_mods(MOD_BIT(KC_RALT));
}

void add_modifiers(bool sft, bool ralt) {
  if(sft && mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
  if(sft && mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
  if(ralt && mods_state.ralt) add_mods(MOD_BIT(KC_RALT));
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
    add_modifiers(true,true);
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
    if(active_held_key->kc[3]) {
      remove_modifiers(true,true);
      add_to_q_release_and_press(active_held_key, 3);
      return KC_NO;
    }
    add_modifiers(true,true);
  } else if(mods_state.lsft || mods_state.rsft) {
    if(active_held_key->kc[1]) {
      remove_modifiers(true,false);
      add_to_q_release_and_press(active_held_key, 1);
      return KC_NO;
    }
    add_modifiers(true,false);
  } else if(mods_state.ralt) {
    if(active_held_key->kc[2]) {
      remove_modifiers(false,true);
      add_to_q_release_and_press(active_held_key, 2);
      return KC_NO;
    }
    add_modifiers(false,true);
  }
  if(active_held_key->i) {
    add_to_q_release_and_press(active_held_key, 0);
  }
  return mod_kc;
}

uint8_t get_kcid_on_key_press(uint16_t* kcs) {
  if(mods_state.other) {
    return 0;
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt && kcs[3]) {
    remove_modifiers(true,true);
    return 3;
  } else if((mods_state.lsft || mods_state.rsft) && kcs[1]) {
    remove_modifiers(true,false);
    return 1;
  } else if(mods_state.ralt && kcs[2]) {
    remove_modifiers(false,true);
    return 2;
  }
  return 0;
}

void fire_events_on_key_release(void) {
  if(mods_state.other) {
    add_modifiers(true,true);
  } else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt) {
    add_modifiers(true,true);
  } else if(mods_state.lsft || mods_state.rsft) {
    add_modifiers(true,false);
  } else if(mods_state.ralt) {
    add_modifiers(false,true);
  }
}

/**
 * No KC_TRANSPARENT allowed on the lowest active layers (most of the time the default layer is the 0th,
 * so: no KC_TRANSPARENT on the default layer)
 */
uint16_t keymap_key_to_keycode_cmv(uint8_t layer, keypos_t key) {
  keyrecord_t* current_record = get_current_record();
  if(
    current_record->event.pressed == cached_key.is_pressed &&
    key.row == cached_key.row && key.col == cached_key.col &&
    (layer == cached_key.layer || (cached_key.transp_res && cached_key.kc != KC_TRANSPARENT))
  ) {
    print("Returning cached keycode.\n");
    if(cached_key.kc == KC_TRANSPARENT) print("Cached keycode is KC_TRANSPARENT.\n");
    return cached_key.kc;
  }
  if(cached_key.transp_res && cached_key.kc != KC_TRANSPARENT) cached_key.transp_res = false;
  print("Processing key...\n");
  cached_key.is_pressed = current_record->event.pressed;
  cached_key.layer = layer;
  cached_key.row = key.row;
  cached_key.col = key.col;
  cached_key.kc = pgm_read_word(&keymaps[(layer)][(key.row)][(key.col)]);
  if(IS_MODIFIER(cached_key.kc)) {
    if(current_record->event.pressed) {
      print("Pressed a modifier.\n");
      if(cached_key.kc == KC_LSFT) mods_state.lsft = true;
      else if(cached_key.kc == KC_RSFT) mods_state.rsft = true;
      else if(cached_key.kc == KC_RALT) mods_state.ralt = true;
      else mods_state.other = true;
      cached_key.kc = get_kc_on_modifier_event(cached_key.kc);
    } else { // If the modifier key is released
      print("Released a modifier.\n");
      if(cached_key.kc == KC_LSFT) mods_state.lsft = false;
      else if(cached_key.kc == KC_RSFT) mods_state.rsft = false;
      else if(cached_key.kc == KC_RALT) mods_state.ralt = false;
      else if((keyboard_report->mods & ~(CHAR_MODS_SET | MOD_BIT(cached_key.kc))) == 0) mods_state.other = false;
      get_kc_on_modifier_event(cached_key.kc);
    }
  } else { // If a non modifier key has been pressed
    get_modified_array_from_key(cached_key.kc, layer, key.row, key.col);
    uint8_t kcid = get_kcid_on_key_press(custom_modified_values_buffer);
    if((cached_key.kc = custom_modified_values_buffer[kcid]) == KC_TRANSPARENT) {
      print("Transparency resolution.\n");
      cached_key.transp_res = true;
    } else {
      if(current_record->event.pressed) {
        print("Pressed a non-modifier key.\n");
        print("Adding a new held key to the array.\n");
        add_held_key((heldkey_t){
          .k = (keypos_t){.row = key.row, .col = key.col},
          .i = kcid,
          .kc = {custom_modified_values_buffer[0],custom_modified_values_buffer[1],custom_modified_values_buffer[2],custom_modified_values_buffer[3]}
        });
      } else { // If the key is released
        fire_events_on_key_release();
        if(get_key_q()->c == -1) { // If the key is NOT executed from the additional queue
          print("Released a non-modifier key.\n");
          if(held_keys.i == -1) return cached_key.kc;
          int8_t released_held_key_id = get_held_key_id(key.row, key.col);
          if(released_held_key_id == -1) return cached_key.kc;
          heldkey_t* released_held_key = &held_keys.all[released_held_key_id];
          cached_key.kc = released_held_key->kc[released_held_key->i];
          if(released_held_key_id == remove_held_key(released_held_key_id) && released_held_key_id) {
            released_held_key = get_active_held_key();
            uint8_t kcid = get_kcid_on_key_press(released_held_key->kc);
            if(released_held_key->i != kcid) {
              add_to_q_release_and_press(released_held_key, kcid);
            }
          }
        } else { // If the key IS executed from the additional queue
          print("Processing the releasing event of a non-modifier key executed from the 'additional keys' queue.\n");
          cached_key.kc = get_key_q()->q[get_key_q()->c].kc;
          remove_held_key(get_held_key_id(key.row, key.col));
        }
      }
      cached_key.kc = translate_keycode(cached_key.kc);
    }
  }
  return cached_key.kc;
}

uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
  return keymap_key_to_keycode_cmv(layer, key);
}

#endif
