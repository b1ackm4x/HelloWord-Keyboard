//
// Created by blackmax on 2022/11/22.
//

#include "rgb_matrix.h"
#include "breathing_anim.h"
#include "colorband_pinwheel_sat_anim.h"
#include "common_inc.h"
#include "rgb_matrix_types.h"
#define NOLED 255

// global
rgb_config_t g_rgb_matrix_config = {.raw = 0};
uint32_t g_rgb_timer = 0;
RGB rgb_buffer[6][25] = {{0}};
uint8_t RGBMatrixIndex[6][15] = {
    {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, NOLED},
    {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
    {43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29},
    {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, NOLED},
    {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, NOLED},
    {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, NOLED, NOLED, NOLED, NOLED,
     NOLED}};

led_config_t g_led_config = {
    {// Key Matrix to LED Index
     {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, NOLED},
     {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28},
     {43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29},
     {44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, NOLED},
     {71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, NOLED},
     {72, 73, 74, 75, 76, 77, 78, 79, 80, 81, NOLED, NOLED, NOLED, NOLED,
      NOLED}},
    {// LED Index to Physical Position
     {0, 0},    {13, 0},   {26, 0},   {39, 0},   {52, 0},   {65, 0},
     {78, 0},   {91, 0},   {104, 0},  {117, 0},  {130, 0},  {143, 0},
     {156, 0},  {195, 0},  {208, 0},  {224, 0},  {0, 12},   {13, 12},
     {26, 12},  {39, 12},  {52, 12},  {65, 12},  {78, 12},  {91, 12},
     {104, 12}, {117, 12}, {130, 12}, {143, 12}, {156, 12}, {169, 12},
     {182, 24}, {195, 12}, {208, 12}, {224, 12}, {0, 24},   {13, 24},
     {26, 24},  {39, 24},  {52, 24},  {65, 24},  {78, 24},  {91, 24},
     {104, 24}, {117, 24}, {130, 24}, {143, 24}, {156, 24}, {169, 24},
     {195, 24}, {208, 24}, {224, 24}, {0, 36},   {13, 36},  {26, 36},
     {39, 36},  {52, 36},  {65, 36},  {78, 36},  {91, 36},  {104, 36},
     {117, 36}, {130, 36}, {143, 36}, {156, 36}, {169, 36}, {0, 48},
     {13, 48},  {26, 48},  {39, 48},  {52, 48},  {65, 48},  {78, 48},
     {91, 48},  {104, 48}, {117, 48}, {130, 48}, {143, 48}, {156, 48},
     {169, 48}, {208, 48}, {0, 64},   {13, 64},  {26, 64},  {78, 64},
     {130, 64}, {143, 64}, {156, 64}, {169, 64}, {195, 64}, {208, 64}},
    {// LED Index to Flag
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
     1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

led_point_t k_rgb_matrix_center = {112, 32};

// internals
static bool suspend_state = false;
static uint8_t rgb_last_enable = UINT8_MAX;
static uint8_t rgb_last_effect = UINT8_MAX;
static effect_params_t rgb_effect_params = {0, LED_FLAG_ALL, false};
static rgb_task_states rgb_task_state = SYNCING;
static last_hit_t last_hit_buffer;
last_hit_t g_last_hit_tracker;

void eeconfig_update_rgb_matrix_default(void) {
  g_rgb_matrix_config.rgb_config.enable = 1;
  g_rgb_matrix_config.rgb_config.mode = RGB_MATRIX_BAND_PINWHEEL_SAT;
  g_rgb_matrix_config.rgb_config.hsv = (HSV){HSV_CORAL};
  g_rgb_matrix_config.rgb_config.speed = 32;
  g_rgb_matrix_config.rgb_config.flags = 1;
};

__attribute__((weak)) uint8_t
rgb_matrix_map_row_column_to_led_kb(uint8_t row, uint8_t column,
                                    uint8_t *led_i) {
  return 0;
}

uint8_t rgb_matrix_map_row_column_to_led(uint8_t row, uint8_t column,
                                         uint8_t *led_i) {
  uint8_t led_count = rgb_matrix_map_row_column_to_led_kb(row, column, led_i);
  uint8_t led_index = g_led_config.matrix_co[row][column];
  if (led_index != NO_LED) {
    led_i[led_count] = led_index;
    led_count++;
  }
  return led_count;
}

void rgb_matrix_set_color(int index, uint8_t red, uint8_t green, uint8_t blue) {
  //  rgb_matrix_driver.set_color(index, red, green, blue);
}

void rgb_matrix_set_color_with_rgb(int index, RGB rgb) {
  HWKeyboard_SetRGB(index, rgb);
}

void rgb_matrix_set_color_all_with_rgb(RGB rgb) {
  for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++)
    HWKeyboard_SetRGB(i, rgb);
}

void rgb_matrix_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
  for (uint8_t i = 0; i < DRIVER_LED_TOTAL; i++)
    rgb_matrix_set_color(i, red, green, blue);
}

static void rgb_task_sync(void) { rgb_task_state = STARTING; }

static void rgb_task_start(void) {
  // reset iter
  rgb_effect_params.iter = 0;
  g_last_hit_tracker = last_hit_buffer;
  // next task
  rgb_task_state = RENDERING;
}

static bool rgb_matrix_none(effect_params_t *params) {
  if (!params->init) {
    return false;
  }

  rgb_matrix_set_color_all(0, 0, 0);
  return false;
}

static void rgb_task_render(uint8_t effect) {
  bool rendering = false;
  struct RGBCONFIGPACKED rgb_matrix_config = g_rgb_matrix_config.rgb_config;
  // effect has changed (such as turn on/off or switch mode)
  rgb_effect_params.init = (effect != rgb_last_effect) ||
                           (rgb_matrix_config.enable != rgb_last_enable);
  if (rgb_effect_params.flags != rgb_matrix_config.flags) {
    rgb_effect_params.flags = rgb_matrix_config.flags;
    rgb_matrix_set_color_all(0, 0, 0);
  }

  // each effect can opt to do calculations
  // and/or request PWM buffer updates.
  switch (effect) {
  case RGB_MATRIX_NONE:
    rendering = rgb_matrix_none(&rgb_effect_params);
    break;
  case RGB_MATRIX_BREATHING:
    rendering = BREATHING(&rgb_effect_params);
    break;
  case RGB_MATRIX_BAND_PINWHEEL_SAT:
    rendering = BAND_PINWHEEL_SAT(&rgb_effect_params);
    break;
  }

  rgb_effect_params.iter++;

  // next task
  if (!rendering) {
    rgb_task_state = FLUSHING;
    if (!rgb_effect_params.init && effect == RGB_MATRIX_NONE) {
      // We only need to flush once if we are RGB_MATRIX_NONE
      rgb_task_state = SYNCING;
    }
  }
}

static void rgb_task_flush(uint8_t effect) {
  // update last trackers after the first full render so we can init over
  // several frames
  rgb_last_effect = effect;
  struct RGBCONFIGPACKED rgb_matrix_config = g_rgb_matrix_config.rgb_config;
  rgb_last_enable = rgb_matrix_config.enable;
  HWKeyboard_SyncLights();
  // update pwm buffers
  //  rgb_matrix_update_pwm_buffers();

  // next task
  rgb_task_state = SYNCING;
}

void rgb_matrix_task(void) {

  // Ideally we would also stop sending zeros to the LED driver PWM buffers
  // while suspended and just do a software shutdown. This is a cheap hack for
  // now.
  bool suspend_backlight = suspend_state;
  struct RGBCONFIGPACKED rgb_matrix_config = g_rgb_matrix_config.rgb_config;

  uint8_t effect = suspend_backlight || !rgb_matrix_config.enable
                       ? 0
                       : rgb_matrix_config.mode;

  switch (rgb_task_state) {
  case STARTING:
    rgb_task_start();
    break;
  case RENDERING:
    rgb_task_render(effect);
    if (effect) {
      //      rgb_matrix_indicators();
      //      rgb_matrix_indicators_advanced(&rgb_effect_params);
    }
    break;
  case FLUSHING:
    rgb_task_flush(effect);
    break;
  case SYNCING:
    rgb_task_sync();
    break;
  }
}
