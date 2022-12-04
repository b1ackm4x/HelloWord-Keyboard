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
