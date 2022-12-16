//
// Created by blackmax on 2022/12/8.
//
#include "rgb_matrix_types.h"
#include "rgb_matrix.h"

#ifndef HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_I_H
#define HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_I_H

typedef HSV (*i_f)(HSV hsv, uint8_t i, uint8_t time);

bool effect_runner_i(effect_params_t* params, i_f effect_func) {
  uint8_t led_min = 0;
  uint8_t led_max = 82;

  uint8_t time = scale16by8(g_rgb_timer, qadd8(g_rgb_matrix_config.rgb_config.speed / 4, 1));
  for (uint8_t i = led_min; i < led_max; i++) {

    RGB rgb = hsv_to_rgb(effect_func(g_rgb_matrix_config.rgb_config.hsv, i, time));
    rgb_matrix_set_color_with_rgb(i, rgb);

  }
  return rgb_matrix_check_finished_leds(led_max);
}


#endif // HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_I_H
