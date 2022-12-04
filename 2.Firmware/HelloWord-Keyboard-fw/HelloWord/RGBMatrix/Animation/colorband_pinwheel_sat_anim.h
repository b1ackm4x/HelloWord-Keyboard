//
// Created by blackmax on 2022/11/29.
//

#ifndef HELLOWORD_KEYBOARD_FW_COLORBAND_PINWHEEL_SAT_ANIM_H
#define HELLOWORD_KEYBOARD_FW_COLORBAND_PINWHEEL_SAT_ANIM_H

#include "math_func.h"
#include "color.h"
#include "effect_runner_dx_dy.h"
#include "rgb_matrix_types.h"

static HSV BAND_PINWHEEL_SAT_math(HSV hsv, int16_t dx, int16_t dy, uint8_t time) {
  hsv.s = scale8(hsv.s - time - atan2_8(dy, dx) * 3, hsv.s);
  return hsv;
}

bool BAND_PINWHEEL_SAT(effect_params_t* params) {
  return effect_runner_dx_dy(params, &BAND_PINWHEEL_SAT_math);
}

#endif // HELLOWORD_KEYBOARD_FW_COLORBAND_PINWHEEL_SAT_ANIM_H
