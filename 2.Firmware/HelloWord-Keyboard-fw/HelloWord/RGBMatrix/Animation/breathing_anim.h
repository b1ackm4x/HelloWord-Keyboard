//
// Created by blackmax on 2022/11/22.
//
#include "color.h"
#include "math_func.h"
#include "rgb_matrix.h"
#include "rgb_matrix_types.h"

bool BREATHING(effect_params_t* params) {
    uint8_t led_min = 0;
    uint8_t led_max = 82;
    HSV      hsv  = g_rgb_matrix_config.rgb_config.hsv;
    uint16_t time = scale16by8(g_rgb_timer, g_rgb_matrix_config.rgb_config.speed / 8);
    hsv.v         = scale8(abs8(sin8(time) - 128) * 2, hsv.v);
    RGB rgb       = hsv_to_rgb(hsv);
    rgb_matrix_set_color_all_with_rgb(rgb);
    return rgb_matrix_check_finished_leds(led_max);
}