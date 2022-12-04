//
// Created by blackmax on 2022/11/22.
//

#ifndef HELLOWORD_KEYBOARD_FW_MATH_FUNC
#define HELLOWORD_KEYBOARD_FW_MATH_FUNC

#include "stdint-gcc.h"

extern uint8_t b_m16_interleave[8];

uint8_t sin8(uint8_t theta);

uint8_t atan2_8(int16_t dy, int16_t dx);

int8_t abs8(int8_t i);

///  scale one byte by a second one, which is treated as
///  the numerator of a fraction whose denominator is 256
///  In other words, it computes i * (scale / 256)
///  4 clocks AVR with MUL, 2 clocks ARM
uint8_t scale8(uint8_t i, uint8_t scale);

/// scale a 16-bit unsigned value by an 8-bit value,
///         considered as numerator of a fraction whose denominator
///         is 256. In other words, it computes i * (scale / 256)
uint16_t scale16by8(uint16_t i, uint8_t scale);

#endif // HELLOWORD_KEYBOARD_FW_MATH_FUNC