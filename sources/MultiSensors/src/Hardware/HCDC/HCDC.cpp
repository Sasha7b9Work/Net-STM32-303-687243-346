// 2026/02/03 10:24:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HCDC/HCDC.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f3xx_hal.h>
#include "usbd_cdc.h"
#include "usbd_desc.h"
#include "usbd_cdc_interface.h"


extern USBD_HandleTypeDef hUSBDDevice;


void HCDC::Init()
{
    GPIO_InitTypeDef GPIO_InitStruct = { 0 };
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF14_USB;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_USB_CLK_ENABLE();

    /* Init Device Library */
    USBD_Init(&hUSBDDevice, &VCP_Desc, 0);

    /* Add Supported Class */
    USBD_RegisterClass(&hUSBDDevice, &USBD_CDC);

    /* Add CDC Interface Class */
    USBD_CDC_RegisterInterface(&hUSBDDevice, &USBD_CDC_fops);

    /* Start Device Process */
    USBD_Start(&hUSBDDevice);
}
