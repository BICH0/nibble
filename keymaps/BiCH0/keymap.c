#include "config.h"
#include QMK_KEYBOARD_H
int coled_brightness = 100;
uint16_t oled_pwf;
bool oled_toff;
bool accent_active;
enum layer_names {
  _ML,
  _FN,
  _FN2,
};

enum custom_keycodes {
  KC_CUST = SAFE_RANGE
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_ML] = LAYOUT_ansi(
            QK_GESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_BSPC, KC_DEL,
    RGB_TOG, KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,    KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
    KC_F20, KC_CAPS, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,    KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
    TT(_FN2), KC_LSFT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,    KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,          KC_UP,   KC_END,
    TT(_FN), KC_LCTL, KC_LGUI, KC_LALT,                   KC_SPC,                    KC_RALT, MO(_FN), KC_RCTL, KC_LEFT,          KC_DOWN, KC_RGHT
  ),
  [_FN] = LAYOUT_ansi(
               KC_GRAVE,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12, _______,  KC_END,
    RGB_M_P, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, UC_LINX,
    KC_F20, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, UC_WINC,
    TT(_FN2), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          RGB_VAI, KC_HOME,
    TO(_ML), _______, _______, _______,                   _______,                   _______, _______, _______, RGB_MOD,          RGB_VAD, RGB_RMOD
  ),
  [_FN2] = LAYOUT_ansi(
               TO(_ML), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,  _______,
    _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    KC_F20, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    TT(_FN), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______,
    TO(_ML), _______, _______, _______,                   _______,                   _______, _______, _______, _______,          _______, _______
  )
};

#ifdef OLED_ENABLE
  #include "oled_display.h"
  oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    oled_timer = timer_read32();
    oled_pwf = timer_read32();
    set_oled_mode(OLED_MODE_IDLE);
    return OLED_ROTATION_180;
  }

  bool oled_task_user(void) {
    oled_set_brightness(coled_brightness);
    if (timer_elapsed(oled_pwf) > 3000 || oled_toff) {
        oled_off();
        oled_toff = true;
    }
    else{
      if (! is_oled_on()){
        oled_on();
      }
      if (timer_elapsed(oled_timer) >= 2000) {
          set_oled_mode(OLED_MODE_IDLE);
      }
      render_frame();
    }
    return false;
  }
#endif

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  process_record_remote_kb(keycode, record);
  oled_pwf = timer_read();
  oled_toff = false;
  switch(keycode) {
    case KC_QUOT:
      if (record->event.pressed) {
        if (get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT))) {
          // Shift está presionado, enviar comilla doble
          tap_code16(KC_DQUO);
        } else {
          if (accent_active){
              accent_active = false;
              tap_code(KC_QUOT);
          }else{
              // Activar el estado de acento
              accent_active = true;
          }
        }
      }
      return false;
    case RGB_TOG:
      if (record->event.pressed) {
        #ifdef OLED_ENABLE
          process_record_keymap_oled(keycode);
        #endif
      }
    break;
    case KC_CUST: //custom macro
      if (record->event.pressed) {
      }
    break;
    default:
      if (accent_active && record->event.pressed) {
        accent_active = false;  // Deactivate accent for the next keypress
        bool caps_on = host_keyboard_led_state().caps_lock || get_mods() & (MOD_BIT(KC_LSFT) | MOD_BIT(KC_RSFT));
        switch (keycode) {
          case KC_A:
            send_unicode_string(caps_on ? "Á" : "á");
            return false;
          case KC_E:
            send_unicode_string(caps_on ? "É" : "é");
            return false;
          case KC_I:
            send_unicode_string(caps_on ? "Í" : "í");
            return false;
          case KC_O:
            send_unicode_string(caps_on ? "Ó" : "ó");
            return false;
          case KC_U:
            send_unicode_string(caps_on ? "Ú" : "ú");
            return false;
          case KC_N:
            send_unicode_string(caps_on ? "Ñ" : "ñ");
            return false;
          case KC_LSFT:
          case KC_RSFT:
          case KC_CAPS:
            accent_active = true;
            return true;
        }
    }
  }
  return true;
}

// RGB config, for changing RGB settings on non-VIA firmwares
void change_RGB(bool clockwise) {
    bool shift = get_mods() & MOD_MASK_SHIFT;
    bool alt = get_mods() & MOD_MASK_ALT;
    bool ctrl = get_mods() & MOD_MASK_CTRL;

    if (clockwise) {
        if (alt) {
            rgblight_increase_hue();
        } else if (ctrl) {
            rgblight_increase_val();
        } else if (shift) {
            rgblight_increase_sat();
        } else {
            rgblight_step();
        }

  } else {
      if (alt) {
            rgblight_decrease_hue();
        } else if (ctrl) {
            rgblight_decrease_val();
        } else if (shift) {
            rgblight_decrease_sat();
        } else {
            rgblight_step_reverse();
        }
    }
}

bool encoder_update_user(uint8_t index, bool clockwise) {
  oled_pwf = timer_read();
  oled_toff = false;
  if (layer_state_is(1)) {
    if (clockwise){
      #ifdef OLED_ENABLE
        if (oled_get_brightness() <= 235){
          coled_brightness = coled_brightness + 20;
        }
      #endif
    }
    else{
      #ifdef OLED_ENABLE
        if (oled_get_brightness() >= 20){
          coled_brightness = coled_brightness - 20;
        }
      #endif
    }
  }
  else {
    if (clockwise) {
      #ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLU);
      #endif
      tap_code(KC_VOLU);
    } else {
      #ifdef OLED_ENABLE
        process_record_encoder_oled(KC_VOLD);
      #endif
      tap_code(KC_VOLD);
    }
  }
    return true;
}

void matrix_init_user(void) {
  // Initialize remote keyboard, if connected (see readme)
  matrix_init_remote_kb();
}

void matrix_scan_user(void) {
  // Scan and parse keystrokes from remote keyboard, if connected (see readme)
  matrix_scan_remote_kb();
}
