//
// Created by blackmax on 2022/12/13.
//

#include "rgb_matrix.h"
#include "stdint-gcc.h"

enum openrgb_command_id {
  id_get_protocol_version = 0x01, // always 0x01
  id_get_keyboard_value = 0x02,
  id_set_keyboard_value = 0x03,
  id_dynamic_keymap_get_keycode = 0x04,
  id_dynamic_keymap_set_keycode = 0x05,
  id_dynamic_keymap_reset = 0x06,
  id_lighting_set_value = 0x07,
  id_lighting_get_value = 0x08,
  id_lighting_save = 0x09,
  id_eeprom_reset = 0x0A,
  id_bootloader_jump = 0x0B,
  id_dynamic_keymap_macro_get_count = 0x0C,
  id_dynamic_keymap_macro_get_buffer_size = 0x0D,
  id_dynamic_keymap_macro_get_buffer = 0x0E,
  id_dynamic_keymap_macro_set_buffer = 0x0F,
  id_dynamic_keymap_macro_reset = 0x10,
  id_dynamic_keymap_get_layer_count = 0x11,
  id_dynamic_keymap_get_buffer = 0x12,
  id_dynamic_keymap_set_buffer = 0x13,
  id_unhandled = 0xFF,
  id_signalrgb_qmk_version = 0x21,
  id_signalrgb_protocol_version = 0x22,
  id_signalrgb_unique_identifier = 0x23,
  id_signalrgb_stream_leds = 0x24,
  id_signalrgb_effect_enable = 0x25,
  id_signalrgb_effect_disable = 0x26,
  id_signalrgb_get_total_leds = 0x27,
  id_signalrgb_get_firmware_type = 0x28,

  id_openrgb_set_color_part1 = 0x30,
  id_openrgb_set_color_part2 = 0x31,
  id_openrgb_set_color_part3 = 0x32,
  id_openrgb_set_color_part4 = 0x33,

};

RGB gen_color_by_offset(uint8_t *data) {
  uint8_t g = data[0];
  uint8_t r = data[1];
  uint8_t b = data[2];
  RGB color = (RGB){r, g, b};
  return color;
}

void raw_hid_receive(uint8_t *data) {
  uint8_t command = data[0];
  switch (command) {
  case id_openrgb_set_color_part1: {
    for (uint8_t idx = 0; idx < 21; idx++) {
      RGB color = gen_color_by_offset( &data[1 + idx * 3]);
      rgb_matrix_set_color_with_rgb(idx, color);
    }
    break;
  }
  case id_openrgb_set_color_part2: {
    for (uint8_t idx = 21; idx < 42; idx++) {
      RGB color = gen_color_by_offset( &data[1 + (idx - 21) * 3]);
      rgb_matrix_set_color_with_rgb(idx, color);
    }
    break;
  }
  case id_openrgb_set_color_part3: {
    for (uint8_t idx = 42; idx < 63; idx++) {
      RGB color = gen_color_by_offset( &data[1 + (idx - 42) * 3]);
      rgb_matrix_set_color_with_rgb(idx, color);
    }
    break;
  }
  case id_openrgb_set_color_part4: {
    for (uint8_t idx = 63; idx < 84; idx++) {
      RGB color = gen_color_by_offset( &data[1 + (idx - 63) * 3]);
      rgb_matrix_set_color_with_rgb(idx, color);
    }
    break;
  }
  default:
    break ;
  }
}

