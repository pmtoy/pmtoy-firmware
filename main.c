/* Includes ------------------------------------------------------------------*/

#include "main.h"
/** @addtogroup STM32F1xx_HAL_Examples
  * @{
  */

/** @addtogroup GPIO_IOToggle
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct_LED,
                         GPIO_InitStruct_BTN;


#ifdef USE_USB_VCP
USBD_HandleTypeDef USBD_Device;
#endif

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);


/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct_LED.Pin = USR_LED_GPIO_PIN;
    GPIO_InitStruct_LED.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct_LED.Pull  = GPIO_PULLDOWN;
    GPIO_InitStruct_LED.Speed = GPIO_SPEED_LOW;

    HAL_GPIO_Init(USR_LED_GPIO_PORT, &GPIO_InitStruct_LED);

    GPIO_InitStruct_BTN.Pin = BTN_GPIO_PIN;
    GPIO_InitStruct_BTN.Mode  = GPIO_MODE_IT_RISING;
    GPIO_InitStruct_BTN.Pull  = GPIO_PULLUP;
    GPIO_InitStruct_BTN.Speed = GPIO_SPEED_LOW;

    HAL_GPIO_Init(BTN_GPIO_PORT, &GPIO_InitStruct_BTN);

    HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

    HAL_GPIO_WritePin(USR_LED_GPIO_PORT, USR_LED_GPIO_PIN, GPIO_PIN_RESET);


#ifdef USE_USB_VCP
    __HAL_REMAP_PIN_ENABLE(HAL_REMAP_PA11_PA12);

    /* Init Device Library */
    USBD_Init(&USBD_Device, &VCP_Desc, 0);

    /* Add Supported Class */
    USBD_RegisterClass(&USBD_Device, &USBD_CDC);

    /* Add CDC Interface Class */
    USBD_CDC_RegisterInterface(&USBD_Device, &USBD_CDC_fops);

    /* Start Device Process */
    USBD_Start(&USBD_Device);
#endif


    while (1) {
        HAL_Delay(200);
    };

    Error_Handler();
    return -1;
}


/**
  * @brief  System Clock Configuration
  * @param  None
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef clkinitstruct = {0};
  RCC_OscInitTypeDef oscinitstruct = {0};
#ifdef USE_USB_VCP
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct = {0};
#endif

  /* Configure PLL ------------------------------------------------------*/
  oscinitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_HSE;
  oscinitstruct.HSIState        = RCC_HSI_ON;
  oscinitstruct.HSEState        = RCC_HSE_ON;
  oscinitstruct.HSICalibrationValue = 16;
  oscinitstruct.PLL.PLLState    = RCC_PLL_ON;
  oscinitstruct.PLL.PLLSource   = RCC_PLLSOURCE_HSE;
  oscinitstruct.PLL.PREDIV      = RCC_PREDIV_DIV1;

  oscinitstruct.PLL.PLLMUL      = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&oscinitstruct)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

#ifdef USE_USB_VCP
  /*Select PLL 48 MHz output as USB clock source */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInitStruct.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
#endif

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  clkinitstruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1);
  clkinitstruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  clkinitstruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  clkinitstruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&clkinitstruct, FLASH_LATENCY_1)!= HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
}

static void Error_Handler(void) {
    while (1) {
        HAL_GPIO_TogglePin(USR_LED_GPIO_PORT, USR_LED_GPIO_PIN);
        HAL_Delay(200);
    }
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


