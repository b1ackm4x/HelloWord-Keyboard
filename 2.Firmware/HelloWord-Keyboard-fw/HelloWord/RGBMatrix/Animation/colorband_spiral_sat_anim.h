//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_SAT_ANIM_H
#define HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_SAT_ANIM_H

#include "math_func.h"
#include "color.h"
#include "effect_runner_dx_dy_dist.h"
#include "rgb_matrix_types.h"

static HSV BAND_SPIRAL_SAT_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint8_t time) {
  hsv.s = scale8(hsv.s + dist - time - atan2_8(dy, dx), hsv.s);
  return hsv;
}

bool BAND_SPIRAL_SAT(effect_params_t* params) {
  return effect_runner_dx_dy_dist(params, &BAND_SPIRAL_SAT_math);
}

#endif // HELLOWORD_KEYBOARD_FW_COLORBAND_SPIRAL_SAT_ANIM_H
