//
// Created by blackmax on 2022/12/8.
//

#ifndef HELLOWORD_KEYBOARD_FW_CYCLE_ALL_MATH_H
#define HELLOWORD_KEYBOARD_FW_CYCLE_ALL_MATH_H

#include "math_func.h"
#include "color.h"
#include "rgb_matrix_types.h"
#include "effect_runner_i.h"

static HSV CYCLE_ALL_math(HSV hsv, uint8_t i, uint8_t time) {
  hsv.h = time;
  return hsv;
}

bool CYCLE_ALL(effect_params_t* params) {
  return effect_runner_i(params, &CYCLE_ALL_math);
}

#endif // HELLOWORD_KEYBOARD_FW_CYCLE_ALL_MATH_H
