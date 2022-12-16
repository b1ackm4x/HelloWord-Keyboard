#include "math_func.h"
#include "stdint-gcc.h"

uint8_t b_m16_interleave[8] = {0, 49, 49, 41, 90, 27, 117, 10};

uint8_t sin8(uint8_t theta) {
  uint8_t offset = theta;
  if (theta & 0x40) {
    offset = (uint8_t)255 - offset;
  }
  offset &= 0x3F; // 0..63

  uint8_t secoffset = offset & 0x0F; // 0..15
  if (theta & 0x40)
    secoffset++;

  uint8_t section = offset >> 4; // 0..3
  uint8_t s2 = section * 2;
  const uint8_t *p = b_m16_interleave;
  p += s2;
  uint8_t b = *p;
  p++;
  uint8_t m16 = *p;

  uint8_t mx = (m16 * secoffset) >> 4;

  int8_t y = mx + b;
  if (theta & 0x80)
    y = -y;

  y += 128;

  return y;
}

uint8_t cos8(uint8_t theta) {
  return sin8( theta + 64);
}

uint8_t atan2_8(int16_t dy, int16_t dx) {
  if (dy == 0) {
    if (dx >= 0)
      return 0;
    else
      return 128;
  }

  int16_t abs_y = dy > 0 ? dy : -dy;
  int8_t a;

  if (dx >= 0)
    a = 32 - (32 * (dx - abs_y) / (dx + abs_y));
  else
    a = 96 - (32 * (dx + abs_y) / (abs_y - dx));

  if (dy < 0)
    return -a; // negate if in quad III or IV
  return a;
}

int8_t abs8(int8_t i) {
  if (i < 0)
    i = -i;
  return i;
}

uint8_t scale8(uint8_t i, uint8_t scale) {
  return (((uint16_t)i) * (1 + (uint16_t)(scale))) >> 8;
}

uint16_t scale16by8(uint16_t i, uint8_t scale) {
  uint16_t result;
  result = (i * scale) / 256;
  return result;
}

uint8_t sqrt16(uint16_t x) {
  if (x <= 1) {
    return x;
  }

  uint8_t low = 1; // lower bound
  uint8_t hi, mid;

  if (x > 7904) {
    hi = 255;
  } else {
    hi = (x >> 5) + 8; // initial estimate for upper bound
  }

  do {
    mid = (low + hi) >> 1;
    if ((uint16_t)(mid * mid) > x) {
      hi = mid - 1;
    } else {
      if (mid == 255) {
        return 255;
      }
      low = mid + 1;
    }
  } while (hi >= low);

  return low - 1;
}

uint8_t qadd8(uint8_t i, uint8_t j) {
  uint16_t t = i + j;
  if (t > 255)
    t = 255;
  return t;
}
int8_t qadd7(int8_t i, int8_t j) {
  int16_t t = i + j;
  if (t > 127)
    t = 127;
  return t;
}


#include<stdlib.h>
uint8_t random8(void) {
//  #define RAND16_SEED  1337
//  uint16_t rand16seed = RAND16_SEED;
//
//  rand16seed = (rand16seed * FASTLED_RAND16_2053) + FASTLED_RAND16_13849;
//  // return the sum of the high and low bytes, for better
//  //  mixing and non-sequential correlation
//  return (uint8_t)(((uint8_t)(rand16seed & 0xFF)) +
//                   ((uint8_t)(rand16seed >> 8)));
  return (uint8_t)rand();
}
