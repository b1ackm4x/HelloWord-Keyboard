//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_VAL_ANIM_H
#define HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_VAL_ANIM_H

#include "math_func.h"
#include "color.h"
#include "effect_runner_dx_dy_dist.h"
#include "rgb_matrix_types.h"

static HSV BAND_SPIRAL_VAL_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
  hsv.v = scale8(hsv.v + dist - time - atan2_8(dy, dx), hsv.v);
  return hsv;
}

bool BAND_SPIRAL_VAL(effect_params_t* params) {
  return effect_runner_dx_dy_dist(params, &BAND_SPIRAL_VAL_math);
}

#endif // HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_VAL_ANIM_H
