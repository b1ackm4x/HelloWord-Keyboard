//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_DX_DY_DIST_H
#define HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_DX_DY_DIST_H

#include "rgb_matrix_types.h"
#include "rgb_matrix.h"
#include "math_func.h"

typedef HSV (*dx_dy_dist_f)(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time);

bool effect_runner_dx_dy_dist(effect_params_t* params, dx_dy_dist_f effect_func) {
  uint8_t led_min = 0;
  uint8_t led_max = 82;

  uint8_t time = scale16by8(g_rgb_timer, g_rgb_matrix_config.rgb_config.speed / 2);
  for (uint8_t i = led_min; i < led_max; i++) {
    int16_t dx   = g_led_config.point[i].x - k_rgb_matrix_center.x;
    int16_t dy   = g_led_config.point[i].y - k_rgb_matrix_center.y;
    uint8_t dist = sqrt16(dx * dx + dy * dy);
    RGB     rgb  = hsv_to_rgb(effect_func(g_rgb_matrix_config.rgb_config.hsv, dx, dy, dist, time));
    rgb_matrix_set_color_with_rgb(i, rgb);
  }
  return rgb_matrix_check_finished_leds(led_max);
}


#endif // HELLOWORD_KEYBOARD_FW_EFFECT_RUNNER_DX_DY_DIST_H
