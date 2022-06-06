#include QMK_KEYBOARD_H

// Additional Keycodes for Custom Macro
enum plaid_keycodes {
  M_01= SAFE_RANGE,
  M_02, M_03, M_04, M_05, M_06, M_07, M_08, M_09, M_10, M_11, M_12, M_13
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/* Qwerty */
[0] = LAYOUT_plaid_grid(
    KC_ESC, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC, 
    KC_TAB, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_ENT, 
    KC_LSFT, KC_GRV, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_DOT, KC_SLSH, KC_RSFT, 
    KC_LCTL, KC_LGUI, KC_LALT, KC_RALT, KC_SPC, LT(1,KC_SPC), LT(2,KC_SPC), KC_SPC, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
), 

/* Lower */
[1] = LAYOUT_plaid_grid(
    A(KC_F4), KC_1, KC_2, KC_3, M_10, M_11, M_12, KC_CIRC, KC_ASTR, KC_LPRN, KC_RPRN, _______, 
    KC_CAPS, KC_4, KC_5, KC_6, M_01, M_02, M_03, KC_MINS, KC_PLUS, KC_EQL, KC_QUOT, _______, 
    _______, KC_7, KC_8, KC_9, M_04, M_05, M_06, KC_LABK, KC_RABK, KC_COMM, KC_BSLS, _______, 
    _______, KC_0, KC_DOT, KC_RCTL, KC_F2, _______, KC_MUTE, KC_DEL, KC_VOLD, KC_PGDN, KC_PGUP, KC_VOLU
), 

/* Raise */
[2] = LAYOUT_plaid_grid(
    KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, 
    KC_NLCK, MEH(KC_B), LCA(KC_S), S(KC_F6), S(KC_F5), LCA(KC_G), C(KC_M), C(KC_J), C(S(KC_I)), C(KC_O), KC_DQUO, _______, 
    _______, LCA(KC_I), C(KC_Z), C(KC_A), C(KC_C), C(S(KC_V)), M_07, M_08, M_09, KC_UNDS, KC_AT, _______, 
    _______, _______, _______, _______, SGUI(KC_S), KC_PSCR, _______, KC_X, C(KC_LEFT), C(KC_DOWN), C(KC_UP), C(KC_RGHT)
)
};

/* OLED */
#define ANIM_SIZE 512

#define IDLE_SPEED 30
#define TYPE_SPEED 50
#define FEVER_SPEED 130

#define ANIM_SPEED_TYPE 200
#define ANIM_SPEED_FEVER 160

uint8_t counter_i = 0;
uint8_t counter_n = 0;

static void render_anim(void) {
  static const char PROGMEM idle[][ANIM_SIZE] = {
    // Sleep
    {
    },
    // Default
    {
    }
  };
  static const char PROGMEM type[][ANIM_SIZE] = {
    // Left
    {
    },
    // Right
    {
    }
  };
  static const char PROGMEM fever[][ANIM_SIZE] = {
    // Fever
    {
    },
    // Fever2
    {
    }
  };
  // Anim
  oled_set_cursor(8,0);
  if(get_current_wpm() <=IDLE_SPEED){
      oled_write_raw_P(idle[0], ANIM_SIZE);
   }
  if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TYPE_SPEED){
      oled_write_raw_P(idle[1], ANIM_SIZE);
  }
  if(get_current_wpm() >=TYPE_SPEED && get_current_wpm() <FEVER_SPEED){
      counter_i = (counter_i + 1) % ANIM_SPEED_TYPE ;
      if(counter_i < ANIM_SPEED_TYPE / 2 ){ counter_n = 0; } else { counter_n = 1; };
      oled_write_raw_P(type[counter_n], ANIM_SIZE);
  }
  if(get_current_wpm() >=FEVER_SPEED){
      counter_i = (counter_i + 1) % ANIM_SPEED_FEVER;
      if(counter_i < ANIM_SPEED_FEVER / 2 ){ counter_n = 0; } else { counter_n = 1; };
      oled_write_raw_P(fever[counter_n], ANIM_SIZE);
  }
}

#ifdef OLED_ENABLE
void oled_clear(void);
void oled_task_user(void) {
    render_anim();
    
  // Texts
    uint8_t n = get_current_wpm();
    char    wpm_str[4];
    wpm_str[3] = '\0';
    wpm_str[2] = '0' + n % 10;
    wpm_str[1] = '0' + (n /= 10) % 10;
    wpm_str[0] = '0' + n / 10;

    oled_set_cursor(0, 0);
    oled_write(wpm_str, false);
    oled_write(" WPN", false);

    oled_set_cursor(1,1);
    switch (get_highest_layer(layer_state)) {
        case 0:
            oled_write_P(PSTR("[QRT]\n"), false);
            break;
        case 1:
            oled_write_P(PSTR("[LWR]\n"), false);
            break;
        case 2:
            oled_write_P(PSTR("[RSE]\n"), false);
            break;
        default:
            oled_write_P(PSTR("[ADJ]\n"), false);
    }
    oled_set_cursor(1,2);
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("[CPS]") : PSTR("      "), false);

    oled_set_cursor(1,3);
    oled_write_P(led_state.num_lock ? PSTR("[NUM]") : PSTR("     "), false);
}
#endif
