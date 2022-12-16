//
// Created by blackmax on 2022/12/7.
//

#include "configurations.h"
#include "Utils/Memory/random_flash_interface.h"
#include "rgb_matrix.h"

void pull_rgb_matrix_config_from_eeprom(void) {
  EEPROM eeprom;
  eeprom.Pull(0, g_rgb_matrix_config);
}

void set_rgb_matrix_config_to_eeprom(void) {
  EEPROM eeprom;
  eeprom.Push(0, g_rgb_matrix_config);
}