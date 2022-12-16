//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_SIN_COS_I_H
#define HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_SIN_COS_I_H

#include "rgb_matrix_types.h"
#include "rgb_matrix.h"
#include "math_func.h"

typedef HSV (*sin_cos_i_f)(HSV hsv, int8_t sin, int8_t cos, uint8_t i, uint8_t time);

bool effect_runner_sin_cos_i(effect_params_t* params, sin_cos_i_f effect_func) {
  uint8_t led_min = 0;
  uint8_t led_max = 82;

  uint16_t time      = scale16by8(g_rgb_timer, g_rgb_matrix_config.rgb_config.speed / 4);
  int8_t   cos_value = cos8(time) - 128;
  int8_t   sin_value = sin8(time) - 128;
  for (uint8_t i = led_min; i < led_max; i++) {
    RGB rgb = hsv_to_rgb(effect_func(g_rgb_matrix_config.rgb_config.hsv, cos_value, sin_value, i, time));
    rgb_matrix_set_color_with_rgb(i, rgb);
  }
  return rgb_matrix_check_finished_leds(led_max);
}


#endif // HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_SIN_COS_I_H
