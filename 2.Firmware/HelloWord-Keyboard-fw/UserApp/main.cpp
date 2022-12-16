#include "common_inc.h"
#include "configurations.h"
#include "hw_keyboard.h"

/* Component Definitions -----------------------------------------------------*/
KeyboardConfig_t config;
HWKeyboard keyboard(&hspi1);

bool switch_action_flag = false;

/* Main Entry ----------------------------------------------------------------*/
void Main()
{

//    EEPROM eeprom;
//    eeprom.Pull(0, config);
//    if (config.configStatus != CONFIG_OK)
//    {
//        // Use default settings
//        config = KeyboardConfig_t{
//            .configStatus = CONFIG_OK,
//            .serialNum=123,
//            .keyMap={}
//        };
//        memset(config.keyMap, -1, 128);
//        eeprom.Push(0, config);
//    }

    // Keyboard Report Start
    HAL_TIM_Base_Start_IT(&htim4);
    pull_rgb_matrix_config_from_eeprom();
    eeconfig_update_rgb_matrix_default();
    set_rgb_matrix_config_to_eeprom();

    while (true)
    {
      rgb_matrix_task();
    }
}

/* Event Callbacks -----------------------------------------------------------*/
extern "C" void OnTimerCallback() // 1000Hz callback
{
    g_rgb_timer ++;
    keyboard.ScanKeyStates();  // Around 40us use 4MHz SPI clk
    keyboard.GetPressedStatus();
    keyboard.ApplyDebounceFilter(300);
    keyboard.Remap(keyboard.FnPressed() ? 2 : 1);  // When Fn pressed use layer-2

    if (!keyboard.FnPressed() || !keyboard.KeyPressed(HWKeyboard::RIGHT_ARROW)) {
      switch_action_flag = false;
    }


    if (keyboard.FnPressed() && keyboard.KeyPressed(HWKeyboard::RIGHT_ARROW) )
    {
      if (!switch_action_flag) {
        switch_action_flag = true;
        right_switch_effect_mode();
      }
    }
    if (keyboard.KeyPressed(HWKeyboard::RIGHT_CTRL) )
    {
        // do something...

        // or trigger some keys...
        keyboard.Press(HWKeyboard::LEFT_CTRL);
    }


    // Report HID key states
    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS,
                               keyboard.GetHidReportBuffer(1),
                               HWKeyboard::KEY_REPORT_SIZE);
}


extern "C"
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi)
{
    keyboard.isRgbTxBusy = false;
}

extern "C"
void HID_RxCpltCallback(uint8_t* _data)
{
    if (_data[0] >= 0x30 && _data[0] <= 0x33 && g_rgb_matrix_config.rgb_config.mode == RGB_MATRIX_OPENRGB) {
        raw_hid_receive(_data);
    }
    else {
        bool cap_lock_flag = _data[1] & 0x02;
        if (cap_lock_flag) {
        keyboard.SetMyRgbBufferByID(84, RGB{255, 255, 255}, 0.5);
        }
        else {
        keyboard.SetMyRgbBufferByID(84, RGB{0, 0, 1   }, 0);
        }
    }
//    keyboard.SyncLights();
}

extern "C" void HWKeyboard_SetRGB(int i, RGB rgb) {
  keyboard.SetMyRgbBufferByID(i, rgb, 0.5);
}

extern "C" void HWKeyboard_SyncLights()
{
  keyboard.SyncLights();
}