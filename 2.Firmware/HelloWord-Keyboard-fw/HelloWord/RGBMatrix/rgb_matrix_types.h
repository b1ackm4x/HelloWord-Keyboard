//
// Created by blackmax on 2022/11/22.
//


#ifndef HELLOWORD_KEYBOARD_FW_RGB_MATRIX_TYPES_H
#define HELLOWORD_KEYBOARD_FW_RGB_MATRIX_TYPES_H


#include "stdint-gcc.h"
#include "color.h"
#include "rgb_matrix.h"
#include <stdbool.h>
#define MATRIX_ROWS 6
#define MATRIX_COLS 15
#define DRIVER_LED_TOTAL 104
#define HAS_FLAGS(bits, flags) ((bits & flags) == flags)
#define HAS_ANY_FLAGS(bits, flags) ((bits & flags) != 0x00)

#define LED_FLAG_ALL 0xFF
#define LED_FLAG_NONE 0x00
#define LED_FLAG_MODIFIER 0x01
#define LED_FLAG_UNDERGLOW 0x02
#define LED_FLAG_KEYLIGHT 0x04
#define LED_FLAG_INDICATOR 0x08

#define LED_HITS_TO_REMEMBER 8
#define NO_LED 255


typedef enum rgb_task_states { STARTING, RENDERING, FLUSHING, SYNCING } rgb_task_states;
typedef uint8_t led_flags_t;

typedef struct PACKED {
    uint8_t     iter;
    led_flags_t flags;
    bool        init;
} effect_params_t;

typedef struct PACKED1 {
    uint8_t x;
    uint8_t y;
} led_point_t;

typedef struct PACKED2 {
    uint8_t     matrix_co[MATRIX_ROWS][MATRIX_COLS];
    led_point_t point[DRIVER_LED_TOTAL];
    uint8_t     flags[DRIVER_LED_TOTAL];
} led_config_t;


struct RGBCONFIGPACKED {
    uint8_t     enable : 2;
    uint8_t     mode : 6;
    HSV         hsv;
    uint8_t     speed; // EECONFIG needs to be increased to support this
    led_flags_t flags;
} ;

typedef union {
    uint64_t raw;
    struct RGBCONFIGPACKED rgb_config;
} rgb_config_t;

typedef struct HITPACKED {
  uint8_t  count;
  uint8_t  x[LED_HITS_TO_REMEMBER];
  uint8_t  y[LED_HITS_TO_REMEMBER];
  uint8_t  index[LED_HITS_TO_REMEMBER];
  uint16_t tick[LED_HITS_TO_REMEMBER];
} last_hit_t;

#endif //HELLOWORD_KEYBOARD_FW_RGB_MATRIX_TYPES_H
