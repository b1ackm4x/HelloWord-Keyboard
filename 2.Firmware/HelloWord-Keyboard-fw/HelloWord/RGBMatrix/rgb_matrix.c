//
// Created by blackmax on 2022/11/22.
//

#include "rgb_matrix.h"
#include "common_inc.h"
#include "rgb_matrix_effects.inc"
#include "rgb_matrix_types.h"
#define NOLED 255

// global
rgb_config_t g_rgb_matrix_config = {.raw = 0};
uint32_t g_rgb_timer = 0;
RGB rgb_buffer[6][25] = {{0}};


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
     {219, 0},  {198, 0},  {184, 0},  {170, 0},  {156, 0},
     {134, 0},  {120, 0},  {106, 0},  {92, 0},   {71, 0},
     {57, 0},   {42, 0},   {28, 0},   {0, 0},

     {0, 15},   {14, 15},  {28, 15},  {42, 15},  {56, 15},
     {70, 15},  {84, 15},  {98, 15},  {112, 15}, {126, 15},
     {140, 15}, {154, 15}, {168, 15}, {189, 15}, {219, 15},

     {219, 27}, {196, 27}, {175, 27}, {165, 27}, {151, 27},
     {137, 27}, {123, 27}, {109, 27}, {95, 27},  {81, 27},
     {67, 27},  {53, 27},  {39, 27},  {25, 27},  {4, 27},

     {6, 40},   {27, 40},  {41, 40},  {55, 40},  {69, 40},
     {83, 40},  {97, 40},  {111, 40}, {125, 40}, {139, 40},
     {153, 40}, {167, 40}, {188, 40}, {219, 40},

     {219, 52}, {200, 58}, {178, 52}, {160, 52}, {144, 52},
     {130, 52}, {116, 52}, {102, 52}, {88, 52},  {74, 52},
     {60, 52},  {46, 52},  {32, 52},  {9, 52},

     {4, 64},   {22, 64},  {40, 64},  {80, 64},  {130, 64},
     {144, 64}, {158, 64}, {178, 64}, {192, 64}, {206, 64}},
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
  g_rgb_matrix_config.rgb_config.mode = RGB_MATRIX_OPENRGB;
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
  for (uint8_t i = 0; i < 82; i++)
    HWKeyboard_SetRGB(i, rgb);
}

void rgb_matrix_set_color_all(uint8_t red, uint8_t green, uint8_t blue) {
  for (uint8_t i = 0; i < 82; i++)
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

//  rgb_matrix_set_color_all(0, 0, 0);
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
  case RGB_MATRIX_BAND_PINWHEEL_VAL:
    rendering = BAND_PINWHEEL_VAL(&rgb_effect_params);
    break;
  case RGB_MATRIX_CYCLE_OUT_IN_DUAL:
    rendering = CYCLE_OUT_IN_DUAL(&rgb_effect_params);
    break ;
  case RGB_MATRIX_CYCLE_ALL:
    rendering = CYCLE_ALL(&rgb_effect_params);
    break ;
  case RGB_MATRIX_GRADIENT_LEFT_RIGHT:
    rendering = GRADIENT_LEFT_RIGHT(&rgb_effect_params);
    break ;
  case RGB_MATRIX_CYCLE_PINWHEEL:
    rendering = CYCLE_PINWHEEL(&rgb_effect_params);
    break ;
  case RGB_MATRIX_BAND_SPIRAL_SAT:
    rendering = BAND_SPIRAL_SAT(&rgb_effect_params);
    break ;
  case RGB_MATRIX_BAND_SPIRAL_VAL:
    rendering = BAND_SPIRAL_VAL(&rgb_effect_params);
    break ;
  case RGB_MATRIX_CYCLE_SPIRAL:
    rendering = CYCLE_SPIRAL(&rgb_effect_params);
    break ;
  case RGB_MATRIX_PIXEL_FLOW:
    rendering = PIXEL_FLOW(&rgb_effect_params);
    break ;
  case RGB_MATRIX_DUAL_BEACON:
    rendering = DUAL_BEACON(&rgb_effect_params);
    break ;
  case RGB_MATRIX_OPENRGB:
    rendering = OPENRGB_RENDERING(&rgb_effect_params);
    break ;
  default:
    rendering = BREATHING(&rgb_effect_params);
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

void left_switch_effect_mode(void) {
  g_rgb_matrix_config.rgb_config.mode = g_rgb_matrix_config.rgb_config.mode > 0 ? g_rgb_matrix_config.rgb_config.mode - 1 : 0;
}

void right_switch_effect_mode(void) {
  g_rgb_matrix_config.rgb_config.mode = g_rgb_matrix_config.rgb_config.mode < 12 ? g_rgb_matrix_config.rgb_config.mode + 1 : 0;
}
