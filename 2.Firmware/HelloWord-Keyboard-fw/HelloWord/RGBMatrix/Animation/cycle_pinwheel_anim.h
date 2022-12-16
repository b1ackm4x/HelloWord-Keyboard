//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_CYCLE_PINWHEEL_ANIM_H
#define HELLOWORD_KEYBOARD_FW_CYCLE_PINWHEEL_ANIM_H
#include "math_func.h"
#include "color.h"
#include "effect_runner_dx_dy.h"
#include "rgb_matrix_types.h"

static HSV CYCLE_PINWHEEL_math(HSV hsv, int16_t dx, int16_t dy, uint8_t time) {
  hsv.h = atan2_8(dy, dx) + time;
  return hsv;
}

bool CYCLE_PINWHEEL(effect_params_t* params) {
  return effect_runner_dx_dy(params, &CYCLE_PINWHEEL_math);
}

#endif // HELLOWORD_KEYBOARD_FW_CYCLE_PINWHEEL_ANIM_H
