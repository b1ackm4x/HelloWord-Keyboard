//
// Created by blackmax on 2022/12/5.
//

#ifndef HELLOWORD_KEYBOARD_FW_CYCLE_OUT_IN_DUAL_ANIM_H
#define HELLOWORD_KEYBOARD_FW_CYCLE_OUT_IN_DUAL_ANIM_H

#include "math_func.h"
#include "color.h"
#include "effect_runner_dx_dy.h"
#include "rgb_matrix_types.h"

static HSV CYCLE_OUT_IN_DUAL_math(HSV hsv, int16_t dx, int16_t dy, uint8_t time) {
  dx           = (k_rgb_matrix_center.x / 2) - abs8(dx);
  uint8_t dist = sqrt16(dx * dx + dy * dy);
  hsv.h        = 3 * dist + time;
  return hsv;
}

bool CYCLE_OUT_IN_DUAL(effect_params_t* params) {
  return effect_runner_dx_dy(params, &CYCLE_OUT_IN_DUAL_math);
}
#endif // HELLOWORD_KEYBOARD_FW_CYCLE_OUT_IN_DUAL_ANIM_H
