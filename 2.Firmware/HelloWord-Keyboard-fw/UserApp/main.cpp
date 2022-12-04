#include "HelloWord/RGBMatrix/Math/math_func.h"
#include "common_inc.h"
#include "configurations.h"
#include "hw_keyboard.h"

/* Component Definitions -----------------------------------------------------*/
KeyboardConfig_t config;
HWKeyboard keyboard(&hspi1);


/* Main Entry ----------------------------------------------------------------*/
void Main()
{
    EEPROM eeprom;
    eeprom.Pull(0, config);
    if (config.configStatus != CONFIG_OK)
    {
        // Use default settings
        config = KeyboardConfig_t{
            .configStatus = CONFIG_OK,
            .serialNum=123,
            .keyMap={}
        };
        memset(config.keyMap, -1, 128);
        eeprom.Push(0, config);
    }

    // Keyboard Report Start
    HAL_TIM_Base_Start_IT(&htim4);
    eeconfig_update_rgb_matrix_default();

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
    keyboard.ApplyDebounceFilter(100);
    keyboard.Remap(keyboard.FnPressed() ? 2 : 1);  // When Fn pressed use layer-2


    if (keyboard.KeyPressed(HWKeyboard::A) )
    {
        // do something...

        // or trigger some keys...
        keyboard.Press(HWKeyboard::B);
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

}

extern "C" void HWKeyboard_SetRGB(int i, RGB rgb) {
  keyboard.SetMyRgbBufferByID(i, rgb, 1);
}

extern "C" void HWKeyboard_SyncLights()
{
  keyboard.SyncLights();
}