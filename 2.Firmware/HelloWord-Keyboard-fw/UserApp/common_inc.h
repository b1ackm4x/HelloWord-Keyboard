#ifndef LOOP_H
#define LOOP_H

#ifdef __cplusplus
extern "C" {
#endif
/*---------------------------- C Scope ---------------------------*/
#include "stdint-gcc.h"
#include "stm32f1xx_hal.h"
#include "tim.h"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "color.h"
#include "rgb_matrix.h"
#include "openrgb_handler.h"



void Main();
void OnUartCmd(uint8_t* _data, uint16_t _len);
void OnTimerCallback();
void HWKeyboard_SetRGB(int i, RGB rgb);
void HWKeyboard_SyncLights();

#ifdef __cplusplus
}


/*---------------------------- C++ Scope ---------------------------*/
#include "HelloWord/Utils/Memory/random_flash_interface.h"


#endif
#endif
