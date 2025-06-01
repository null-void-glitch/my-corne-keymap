// Definitions
#define DEBUG
#define PRINT_LAYER_STATE
#include QMK_KEYBOARD_H
#define frame_size 512
#define EEPROM_ADDR_BRIGHTNESS 0x10
#define logo_glitch_count 5
#define logo_glitch_dirty_count 2
#define text_glitch_count 7
#define text_glitch_dirty_count 3

// Includes
#include "oled_driver.h"
#include "oled_assets.h"
#include "eeprom.h"

// Custom keys
enum custom_keycodes {
    OLED_UP = SAFE_RANGE,
    OLED_DN,
    OLED_BRIGHTNESS_TOGGLE
};

// Keymap (after flash customizable with VIA) add more layers if needed
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT,   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

    [1] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_TAB,    KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                         KC_6,    KC_7,    KC_8,    KC_9,    KC_0, KC_MINS,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT, KC_NUBS, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_BSLS,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LALT, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [2] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
       KC_ESC,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                        KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10, KC_LBRC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,   KC_UP, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_MINS,  KC_EQL, KC_LBRC, KC_RBRC, KC_BSLS,  KC_GRV,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, XXXXXXX,                      KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_PIPE, KC_TILD,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [3] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      QK_BOOT, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLU,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_BTN1, KC_MS_U, KC_BTN2, KC_WH_U, XXXXXXX, XXXXXXX,                      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_VOLD,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_MS_L, KC_MS_D, KC_MS_R, KC_WH_D, XXXXXXX, XXXXXXX,                      OLED_UP, OLED_DN, XXXXXXX, KC_MPRV, KC_MPLY, KC_MNXT,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};

// Brightness handling
static uint8_t oled_brightness_master = 0x7F;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    if (is_keyboard_master()) {
        switch (keycode) {
            case OLED_UP:
                oled_brightness_master = oled_brightness_master + 0x10 > 0xFF ? 0xFF : oled_brightness_master + 0x10;
                eeprom_update_byte((uint8_t*)EEPROM_ADDR_BRIGHTNESS, oled_brightness_master);
                return false;
            case OLED_DN:
                oled_brightness_master = oled_brightness_master < 0x10 ? 0x00 : oled_brightness_master - 0x10;
                eeprom_update_byte((uint8_t*)EEPROM_ADDR_BRIGHTNESS, oled_brightness_master);
                return false;
            case OLED_BRIGHTNESS_TOGGLE:
                oled_brightness_master = (oled_brightness_master == 0x00) ? 0x7F : 0x00;
                eeprom_update_byte((uint8_t*)EEPROM_ADDR_BRIGHTNESS, oled_brightness_master);
                return false;
        }
    }
    return true;
}

// Main WPM rendering
void render_wpm(void) {
    uint8_t wpm = get_current_wpm();
    char buf[4];
    snprintf(buf, sizeof(buf), "%3d", wpm);

    oled_set_cursor(0, 12);            // bottom of the screen (adjust if needed)
    oled_write_P(PSTR("WPM:"), false); // label
    oled_set_cursor(0, 14);            // offset to draw digits after label
    oled_write(buf, false);            // draw the number
}

void matrix_scan_user(void) {
    // this keeps the WPM calculation running
    get_current_wpm();
}

typedef struct {
    bool glitch;
    bool dirty;
    uint8_t glitch_count;
    uint32_t last_frame_time;
} oled_anim_state_t;

static oled_anim_state_t anim_text = { false, false, 0, 0 };
static oled_anim_state_t anim_logo = { false, false, 0, 0 };

bool oled_task_user(void) {
    const uint16_t frame_delay = 200;
    uint32_t now = timer_read();
    // LEFT OLED (Logo)
    if (is_keyboard_master()) {
        oled_set_brightness(oled_brightness_master);
        if (now - anim_logo.last_frame_time > frame_delay) {
            anim_logo.last_frame_time = now;

            const char *frame;

            if (anim_logo.glitch && anim_logo.glitch_count > 0) {
                anim_logo.glitch_count--;

                if (anim_logo.dirty || (rand() % 2)) {
                    frame = logo_glitch_dirty[rand() % logo_glitch_dirty_count];
                } else {
                    frame = logo_glitch[rand() % logo_glitch_count];
                }

                if (anim_logo.glitch_count == 0) {
                    anim_logo.glitch = false;
                }
            } else {
                frame = logo_clean;

                if (rand() % 20 == 0) {
                    anim_logo.glitch = true;
                    anim_logo.glitch_count = 1 + rand() % 4;
                    anim_logo.dirty = anim_logo.glitch_count > 2;
                }
            }
            oled_set_brightness(oled_brightness_master);
            oled_clear();                    // Clear before drawing
            oled_write_raw_P(frame, 386);    // ✅ frame is always valid now
            render_wpm();                    // Draw WPM below
        }

        // RIGHT OLED (Text)
    } else {
        uint8_t brightness = eeprom_read_byte((uint8_t*)EEPROM_ADDR_BRIGHTNESS);
            oled_set_brightness(brightness);
        if (now - anim_text.last_frame_time > frame_delay) {
            anim_text.last_frame_time = now;

            const char *frame;

            if (anim_text.glitch && anim_text.glitch_count > 0) {
                anim_text.glitch_count--;

                if (anim_text.dirty || (rand() % 2)) {
                    frame = text_glitch_dirty[rand() % text_glitch_dirty_count];
                } else {
                    frame = text_glitch[rand() % text_glitch_count];
                }

                if (anim_text.glitch_count == 0) {
                    anim_text.glitch = false;
                }
            } else {
                frame = text_clean;

                if (rand() % 30 == 0) {
                    anim_text.glitch = true;
                    anim_text.glitch_count = 1 + rand() % 4;
                    anim_text.dirty = anim_text.glitch_count > 2;
                }
            }
            oled_write_raw_P(frame, frame_size);
        }
    }
    return false;
}
