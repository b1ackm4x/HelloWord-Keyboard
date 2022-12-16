//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_PIXEL_FLOW_ANIM_H
#define HELLOWORD_KEYBOARD_FW_PIXEL_FLOW_ANIM_H

#include "rgb_matrix_types.h"
#include "rgb_matrix.h"
#include "math_func.h"


static bool PIXEL_FLOW(effect_params_t* params) {

  uint8_t led_min = 0;
  uint8_t led_max = 82;
  // LED state array
  static RGB led[DRIVER_LED_TOTAL];

  static uint32_t wait_timer = 0;
  if (wait_timer > g_rgb_timer) {
    return false;
  }

  inline uint32_t interval(void) {
    return 3000 / scale16by8(qadd8(g_rgb_matrix_config.rgb_config.speed, 16), 16);
  }


  if (params->init) {
    // Clear LEDs and fill the state array
    rgb_matrix_set_color_all_with_rgb((RGB){0, 0, 0});
    for (uint8_t j = 0; j < led_max; ++j) {
      led[j] = (random8() & 2) ? (RGB){0, 0, 0} : hsv_to_rgb((HSV){random8(), qadd8(random8() >> 1, 127), g_rgb_matrix_config.rgb_config.hsv.v});
    }
  }

  // Light LEDs based on state array
  for (uint8_t i = led_min; i < led_max; ++i) {
    rgb_matrix_set_color_with_rgb(i, led[i]);
  }

  if (!rgb_matrix_check_finished_leds(led_max)) {
    // Shift LED state forward
    for (uint8_t j = 0; j < led_max - 1; ++j) {
      led[j] = led[j + 1];
    }
    // Fill last LED
    led[led_max - 1] = (random8() & 2) ? (RGB){0, 0, 0} : hsv_to_rgb((HSV){random8(), qadd8(random8() >> 1, 127), g_rgb_matrix_config.rgb_config.hsv.v});
    // Set pulse timer
    wait_timer = g_rgb_timer + interval();
  }

  return rgb_matrix_check_finished_leds(led_max);
}

#endif // HELLOWORD_KEYBOARD_FW_PIXEL_FLOW_ANIM_H
