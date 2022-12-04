//
// Created by blackmax on 2022/11/24.
//

#ifndef HELLOWORD_KEYBOARD_FW_COLOR_H
#define HELLOWORD_KEYBOARD_FW_COLOR_H

#define HSV_AZURE       132, 102, 255
#define HSV_BLACK         0,   0,   0
#define HSV_BLUE        170, 255, 255
#define HSV_CHARTREUSE   64, 255, 255
#define HSV_CORAL        11, 176, 255
#define HSV_CYAN        128, 255, 255
#define HSV_GOLD         36, 255, 255
#define HSV_GOLDENROD    30, 218, 218
#define HSV_GREEN        85, 255, 255
#define HSV_MAGENTA     213, 255, 255
#define HSV_ORANGE       21, 255, 255
#define HSV_PINK        234, 128, 255
#define HSV_PURPLE      191, 255, 255
#define HSV_RED           0, 255, 255
#define HSV_SPRINGGREEN 106, 255, 255
#define HSV_TEAL        128, 255, 128
#define HSV_TURQUOISE   123,  90, 112
#define HSV_WHITE         0,   0, 255
#define HSV_YELLOW       43, 255, 255
#define HSV_OFF         HSV_BLACK

#include "stdint-gcc.h"


typedef struct {
  uint8_t h;
  uint8_t s;
  uint8_t v;
} HSV;

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RGB;
RGB hsv_to_rgb(HSV hsv);;
#endif //HELLOWORD_KEYBOARD_FW_COLOR_H
