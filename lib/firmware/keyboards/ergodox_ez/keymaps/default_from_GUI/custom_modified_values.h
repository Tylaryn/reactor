#ifndef _CUSTOM_MODIFIED_VALUES_H_
#define _CUSTOM_MODIFIED_VALUES_H_

/*
 * Feature size: 2578 bytes
 * Will probably be able to make it lighter after sharing it with others who will improve it
 * + Code isn't as optimized as it could be
 *
 */

#ifndef CUSTOM_MODIFIED_VALUES_ENABLE
#define CUSTOM_MODIFIED_VALUES_ENABLE
#endif
#ifndef NUMBER_OF_HELD_KEYS
#define NUMBER_OF_HELD_KEYS 8
#endif

#include <stdint.h>
#include <stdbool.h>
#include "action.h"
#include "action_util.h"
#include "keyboard.h"
#include "char_to_keycode.h"

#define IS_MODIFIER(kc) ((kc) >= KC_LCTRL && (kc) <= KC_RGUI)
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
  uint8_t lsft :2;
  uint8_t rsft :2;
  uint8_t ralt :2;
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
uint16_t cmv_buffer[4] = {0,0,0,0};


heldkey_t* get_active_held_key(bool only_non_modifier) {
  if(held_keys.i == -1) return NULL;
  if(!only_non_modifier) return &(held_keys.all[held_keys.i]);
  int8_t i = held_keys.i;
  for(; i >= 0; i--) {
    heldkey_t* key = &(held_keys.all[i]);
    if(!IS_MODIFIER(key->kc[key->i])) return key;
  }
  return NULL;
}

bool add_held_key(heldkey_t new_key) {
  if(held_keys.i == NUMBER_OF_HELD_KEYS - 1) return false;
  held_keys.all[++(held_keys.i)] = new_key;
  return true;
}

heldkey_t* remove_held_key(uint8_t id) {
  // print("Removing a held key from the array.\n");
  uint8_t i = 0;
  for(; i <= held_keys.i; i++) {
    if(i > id) {
      held_keys.all[i - 1] = held_keys.all[i];
    }
  }
  held_keys.i = i-2;
  return get_active_held_key(true);
}

int8_t get_held_key_id(uint8_t row, uint8_t col) {
  uint8_t i = 0;
  for(; i <= held_keys.i; i++) {
    heldkey_t* key = &(held_keys.all[i]);
    if(key->k.row == row && key->k.col == col) {
      return i;
    }
  }
  return -1;
}


bool set_cmv_buffer(uint16_t kc_default, uint16_t kc_shifted, uint16_t kc_altgred, uint16_t kc_sftralt) {
  cmv_buffer[0] = kc_default;
  cmv_buffer[1] = kc_shifted;
  cmv_buffer[2] = kc_altgred;
  cmv_buffer[3] = kc_sftralt;
  return true;
}


/* COL-ROW (not spatial but QMK internal use) (in KEYMAP() k#COL#ROW)
 * ,--------------------------------------------------.           ,--------------------------------------------------.
 * |   0-0  | 0-1  | 0-2  | 0-3  | 0-4  | 0-5  | 0-6  |           |  0-7 |  0-8 |  0-9 |  0-A |  0-B |  0-C |   0-D  |
 * |--------+------+------+------+------+-------------|           |------+------+------+------+------+------+--------|
 * |   1-0  | 1-1  | 1-2  | 1-3  | 1-4  | 1-5  | 1-6  |           |  1-7 |  1-8 |  1-9 |  1-A |  1-B |  1-C |   1-D  |
 * |--------+------+------+------+------+------|      |           |      |------+------+------+------+------+--------|
 * |   2-0  | 2-1  | 2-2  | 2-3  | 2-4  | 2-5  |------|           |------|  2-8 |  2-9 |  2-A |  2-B |  2-C |   2-D  |
 * |--------+------+------+------+------+------| 3-6  |           |  3-7 |------+------+------+------+------+--------|
 * |   3-0  | 3-1  | 3-2  | 3-3  | 3-4  | 3-5  |      |           |      |  3-8 |  3-9 |  3-A |  3-B |  3-C |   3-D  |
 * `--------+------+------+------+------+-------------'           `-------------+------+------+------+------+--------'
 *   | 4-0  | 4-1  | 4-2  | 4-3  | 4-4  |                                       |  4-9 |  4-A |  4-B |  4-C |  4-D |
 *   `----------------------------------'                                       `----------------------------------'
 *                                        ,-------------.       ,-------------.
 *                                        | 5-5  | 5-6  |       |  5-7 |  5-8 |
 *                                 ,------|------|------|       |------+------+------.
 *                                 |      |      | 5-4  |       |  5-9 |      |      |
 *                                 | 5-3  | 5-2  |------|       |------|  5-B |  5-A |
 *                                 |      |      | 5-1  |       |  5-C |      |      |
 *                                 `--------------------'       `--------------------'
 */
__attribute__ ((weak))
bool keycodes_for_key(uint16_t default_kc, uint8_t layer, uint8_t row, uint8_t col) {
  switch(layer) {
    case 0:
      switch(col) {
        case 0:
          switch(row) {
            case 0:
              return set_cmv_buffer(default_kc, UC(0x27), 0, UC(0x41));
          }
      }
  }
  set_cmv_buffer(default_kc, 0, 0, 0);
  return false;
}

uint16_t* get_modified_values_for_key(uint16_t default_kc, uint8_t layer, uint8_t row, uint8_t col) {
  keycodes_for_key(default_kc, layer, row, col);
  return cmv_buffer;
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

void update_modifiers_depending_on_kcid(uint8_t kcid) {
  if(kcid) {
    if(mods_state.lsft) del_mods(MOD_BIT(KC_LSFT));
    if(mods_state.rsft) del_mods(MOD_BIT(KC_RSFT));
    if(mods_state.ralt) del_mods(MOD_BIT(KC_RALT));
  } else {
    if(mods_state.lsft) add_mods(MOD_BIT(KC_LSFT));
    if(mods_state.rsft) add_mods(MOD_BIT(KC_RSFT));
    if(mods_state.ralt) add_mods(MOD_BIT(KC_RALT));
  }
}

void add_to_q_release_and_press(heldkey_t* active_held_key, uint8_t new_kcid) {
  // print("add_to_q_release_and_press.\n");
  keyevent_describer_t ke_descr = {
    .row = active_held_key->k.row,
    .col = active_held_key->k.col,
    .p = false,
    .kcid = active_held_key->i
  };
  push_key_to_q(ke_descr);
  ke_descr.p = true;
  ke_descr.kcid = new_kcid;
  push_key_to_q(ke_descr);
}

uint8_t get_kcid(uint16_t* kcs) {
  if(mods_state.other) return 0;
  else if((mods_state.lsft || mods_state.rsft) && mods_state.ralt && kcs[3]) return 3;
  else if((mods_state.lsft || mods_state.rsft) && kcs[1]) return 1;
  else if(mods_state.ralt && kcs[2]) return 2;
  return 0;
}

void add_modifiers_depending_on_mods_state(void) {
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
  bool is_pressed = get_current_record()->event.pressed;
  if(
    is_pressed == cached_key.is_pressed &&
    key.row == cached_key.row && key.col == cached_key.col &&
    (layer == cached_key.layer || (cached_key.transp_res && cached_key.kc != KC_TRANSPARENT))
  ) {
    // print("Returning cached keycode.\n");
    return cached_key.kc;
  }
  if(cached_key.transp_res && cached_key.kc != KC_TRANSPARENT) cached_key.transp_res = false;
  // print("Processing key...\n");
  cached_key.is_pressed = is_pressed;
  cached_key.layer = layer;
  cached_key.row = key.row;
  cached_key.col = key.col;
  if(is_pressed) {
    cached_key.kc = pgm_read_word(&keymaps[layer][key.row][key.col]);
    if(cached_key.transp_res) {
      heldkey_t* active_held_key = get_active_held_key(false);
      if((cached_key.kc = get_modified_values_for_key(cached_key.kc, layer, key.row, key.col)[active_held_key->i]) == KC_TRANSPARENT) {
        return KC_TRANSPARENT;
      }
      active_held_key->kc[active_held_key->i] = cached_key.kc;
    }
    uint8_t kcid;
    if(IS_MODIFIER(cached_key.kc)) {
      // print("Pressed a modifier key.\n");
      if(cached_key.kc == KC_LSFT && mods_state.lsft != 3) mods_state.lsft++;
      else if(cached_key.kc == KC_RSFT && mods_state.rsft != 3) mods_state.rsft++;
      else if(cached_key.kc == KC_RALT && mods_state.ralt != 3) mods_state.ralt++;
      else mods_state.other = true;
      heldkey_t* active_key = get_active_held_key(true);
      if(active_key != NULL && active_key->i != (kcid = get_kcid(active_key->kc))) {
        add_to_q_release_and_press(active_key, kcid);
      }
      kcid = 0;
      set_cmv_buffer(cached_key.kc,0,0,0);
    } else {
      if(get_key_q()->c == -1) {
        kcid = get_kcid(get_modified_values_for_key(cached_key.kc, layer, key.row, key.col));
      } else {
        kcid = get_key_q()->q[get_key_q()->c].kcid;
        get_modified_values_for_key(cached_key.kc, layer, key.row, key.col);
      }
      update_modifiers_depending_on_kcid(kcid);
    }
    if(!cached_key.transp_res) {
      add_held_key((heldkey_t){
        .k = (keypos_t){.row = key.row, .col = key.col},
        .i = kcid,
        .kc = {cmv_buffer[0],cmv_buffer[1],cmv_buffer[2],cmv_buffer[3]}
      });
      if((cached_key.kc = cmv_buffer[kcid]) == KC_TRANSPARENT) {
        cached_key.transp_res = true;
      }
    }
  } else {
    cached_key.kc = KC_NO;
    if(get_key_q()->c == -1) {
      // print("Released a key.\n");
      if(held_keys.i == -1) return cached_key.kc;
      int8_t released_held_key_id = get_held_key_id(key.row, key.col);
      if(released_held_key_id == -1) return cached_key.kc;
      bool process_new_held_key = true;
      heldkey_t* released_held_key = &held_keys.all[released_held_key_id];
      if(IS_MODIFIER(released_held_key->kc[0])) {
        // print("Key released is a modifier.\n");
        cached_key.kc = released_held_key->kc[0];
        if(cached_key.kc == KC_LSFT && mods_state.lsft != 0) mods_state.lsft--;
        else if(cached_key.kc == KC_RSFT && mods_state.rsft != 0) mods_state.rsft--;
        else if(cached_key.kc == KC_RALT && mods_state.ralt != 0) mods_state.ralt--;
        else if((keyboard_report->mods & ~(CHAR_MODS_SET | MOD_BIT(cached_key.kc))) == 0) mods_state.other = false;
      } else {
        cached_key.kc = released_held_key->kc[released_held_key->i];
        if(released_held_key != get_active_held_key(true)) process_new_held_key = false;
      }
      released_held_key = remove_held_key(released_held_key_id);
      if(process_new_held_key) {
        if(released_held_key == NULL) {
          add_modifiers_depending_on_mods_state();
        } else {
          uint8_t kcid = get_kcid(released_held_key->kc);
          if(released_held_key->i != kcid) {
            add_to_q_release_and_press(released_held_key, kcid);
          } else {
            update_modifiers_depending_on_kcid(kcid);
          }
        }
      }
    } else {
      // print("Processing the releasing event of a non-modifier key executed from the 'additional keys' queue.\n");
      int8_t key_id = get_held_key_id(key.row, key.col);
      if(key_id != -1) {
        cached_key.kc = held_keys.all[key_id].kc[(get_key_q()->q[get_key_q()->c].kcid)];
        remove_held_key(key_id);
      }
    }
  }
  cached_key.kc = translate_keycode(cached_key.kc);
  return cached_key.kc;
}

uint16_t keymap_key_to_keycode(uint8_t layer, keypos_t key) {
  return keymap_key_to_keycode_cmv(layer, key);
}

#endif
