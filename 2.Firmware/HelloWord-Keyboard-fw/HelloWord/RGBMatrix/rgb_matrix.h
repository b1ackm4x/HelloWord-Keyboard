//
// Created by blackmax on 2022/11/22.
//

#ifndef HELLOWORD_KEYBOARD_FW_RGB_MATRIX_H
#define HELLOWORD_KEYBOARD_FW_RGB_MATRIX_H


#include "rgb_matrix_types.h"

enum rgb_matrix_effects {
  RGB_MATRIX_NONE = 0,

  RGB_MATRIX_GRADIENT_LEFT_RIGHT, // Static gradient left to right, speed controls how much gradient changes
  RGB_MATRIX_BREATHING,           // Single hue brightness cycling animation

  RGB_MATRIX_BAND_PINWHEEL_SAT, // Single hue 3 blade spinning pinwheel fades saturation
  RGB_MATRIX_BAND_PINWHEEL_VAL, // Single hue 3 blade spinning pinwheel fades brightness
  RGB_MATRIX_BAND_SPIRAL_SAT,   // Single hue spinning spiral fades saturation
  RGB_MATRIX_BAND_SPIRAL_VAL,   // Single hue spinning spiral fades brightness
  RGB_MATRIX_CYCLE_ALL, // Full keyboard solid hue cycling through full gradient

  RGB_MATRIX_CYCLE_OUT_IN_DUAL,      // Full dual gradients scrolling out to in
  RGB_MATRIX_CYCLE_PINWHEEL, // Full gradient spinning pinwheel around center of keyboard
  RGB_MATRIX_CYCLE_SPIRAL,   // Full gradient spinning spiral around center of keyboard
  RGB_MATRIX_DUAL_BEACON,    // Full gradient spinning around center of keyboard
  RGB_MATRIX_PIXEL_FLOW, // Pulsing RGB flow along LED wiring with random hues
  RGB_MATRIX_OPENRGB,
  RGB_MATRIX_RAINBOW_MOVING_CHEVRON, // Full gradient Chevron shapped scrolling left to right
  RGB_MATRIX_CYCLE_LEFT_RIGHT,       // Full gradient scrolling left to right
  RGB_MATRIX_CYCLE_UP_DOWN,          // Full gradient scrolling top to bottom
  RGB_MATRIX_CYCLE_OUT_IN,           // Full gradient scrolling out to in
  RGB_MATRIX_BAND_SAT, // Single hue band fading saturation scrolling left to right
  RGB_MATRIX_BAND_VAL, // Single hue band fading brightness scrolling left to right
  RGB_MATRIX_SOLID_COLOR, // Static single hue, no speed support
  RGB_MATRIX_ALPHAS_MODS,     // Static dual hue, speed is hue for secondary hue
  RGB_MATRIX_GRADIENT_UP_DOWN, // Static gradient top to bottom, speed controls how much gradient changes
  RGB_MATRIX_RAINBOW_BEACON, // Full tighter gradient spinning around center of keyboard
  RGB_MATRIX_RAINBOW_PINWHEELS,   // Full dual gradients spinning two halfs of keyboard
  RGB_MATRIX_RAINDROPS,           // Randomly changes a single key's hue
  RGB_MATRIX_JELLYBEAN_RAINDROPS, // Randomly changes a single key's hue and saturation
  RGB_MATRIX_HUE_BREATHING, // Hue shifts up a slight ammount at the same time, then shifts back
  RGB_MATRIX_HUE_PENDULUM,  // Hue shifts up a slight ammount in a wave to the right, then back to the left
  RGB_MATRIX_HUE_WAVE, // Hue shifts up a slight ammount and then back down in a wave to the right
  RGB_MATRIX_PIXEL_FRACTAL, // Single hue fractal filled keys pulsing horizontally out to edges
  RGB_MATRIX_PIXEL_RAIN, // Randomly light keys with random hues

};


extern rgb_config_t g_rgb_matrix_config;
extern uint32_t g_rgb_timer;
extern led_point_t k_rgb_matrix_center;
extern led_config_t g_led_config;

void eeconfig_update_rgb_matrix_default(void);
void rgb_matrix_task(void);
void left_switch_effect_mode(void);
void right_switch_effect_mode(void);

static inline bool rgb_matrix_check_finished_leds(uint8_t led_idx) {
  return led_idx < 80;
};

void rgb_matrix_set_color(int, uint8_t, uint8_t, uint8_t);
void rgb_matrix_set_color_with_rgb(int index, RGB rgb);
void rgb_matrix_set_color_all_with_rgb(RGB rgb);


#endif // HELLOWORD_KEYBOARD_FW_RGB_MATRIX_H
