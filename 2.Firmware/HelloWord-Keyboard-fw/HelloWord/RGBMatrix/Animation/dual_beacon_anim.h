//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_DUAL_BEACON_ANIM_H
#define HELLOWORD_KEYBOARD_FW_DUAL_BEACON_ANIM_H

#include "math_func.h"
#include "color.h"
#include "effect_runner_sin_cos_i.h"
#include "rgb_matrix_types.h"

static HSV DUAL_BEACON_math(HSV hsv, int8_t sin, int8_t cos, uint8_t i, uint8_t time) {
  hsv.h += ((g_led_config.point[i].y - k_rgb_matrix_center.y) * cos + (g_led_config.point[i].x - k_rgb_matrix_center.x) * sin) / 128;
  return hsv;
}

bool DUAL_BEACON(effect_params_t* params) {
  return effect_runner_sin_cos_i(params, &DUAL_BEACON_math);
}


#endif // HELLOWORD_KEYBOARD_FW_DUAL_BEACON_ANIM_H
