//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_GRADIENT_LEFT_RIGHT_ANIM_H
#define HELLOWORD_KEYBOARD_FW_GRADIENT_LEFT_RIGHT_ANIM_H
#include "rgb_matrix_types.h"
#include "rgb_matrix.h"

bool GRADIENT_LEFT_RIGHT(effect_params_t* params) {
  uint8_t led_min = 0;
  uint8_t led_max = 82;

  HSV     hsv   = g_rgb_matrix_config.rgb_config.hsv;
  uint8_t scale = scale8(64, g_rgb_matrix_config.rgb_config.speed);
  for (uint8_t i = led_min; i < led_max; i++) {

    // The x range will be 0..224, map this to 0..7
    // Relies on hue being 8-bit and wrapping
    hsv.h   = g_rgb_matrix_config.rgb_config.hsv.h + (scale * g_led_config.point[i].x >> 5);
    RGB rgb = hsv_to_rgb(hsv);
    rgb_matrix_set_color_with_rgb(i, rgb);
  }
  return rgb_matrix_check_finished_leds(led_max);
}

#endif // HELLOWORD_KEYBOARD_FW_GRADIENT_LEFT_RIGHT_ANIM_H
