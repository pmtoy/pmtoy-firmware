#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx_hal_rcc_ex.h"
#include "stm32f0xx_it.h"

#ifdef USE_USB_VCP
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_cdc.h"
#include "usbd_cdc_interface.h"
#endif

/* Private macro -------------------------------------------------------------*/

#define TFT_LED_GPIO_PORT GPIOB
#define TFT_LED_GPIO_PIN GPIO_PIN_1

#define BTN_GPIO_PORT GPIOA
#define BTN_GPIO_PIN GPIO_PIN_0

#define USR_LED_GPIO_PORT GPIOA
#define USR_LED_GPIO_PIN GPIO_PIN_4

#endif /* __MAIN_H */
