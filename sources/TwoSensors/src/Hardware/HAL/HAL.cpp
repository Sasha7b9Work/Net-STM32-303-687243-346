// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <stm32f3xx_hal.h>
#include <cstring>


static void SystemClock_Config();


void HAL::Init()
{
    HAL_Init();
    SystemClock_Config();

    __HAL_RCC_PWR_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    __HAL_RCC_SPI1_CLK_ENABLE();
    __HAL_RCC_RTC_ENABLE();
    __HAL_RCC_I2C1_CLK_ENABLE();
    __HAL_RCC_ADC1_CLK_ENABLE();

//    __HAL_RCC_WWDG_CLK_ENABLE();

//    HAL_I2C1::Init();

    HAL_RTC::Init();

    HAL_ADC::Init();

    HAL_SPI1::Init();
}


void HAL::Delay(unsigned int timeMS)
{
    HAL_Delay(timeMS);
}


static void SystemClock_Config()
{
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };
    RCC_PeriphCLKInitTypeDef PeriphClkInit = { 0 };

    HAL_PWR_EnableBkUpAccess();
    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        HAL::ErrorHandler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
        RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
    {
        HAL::ErrorHandler();
    }

    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB | RCC_PERIPHCLK_USART1 |
        RCC_PERIPHCLK_I2C1 | RCC_PERIPHCLK_RTC | RCC_PERIPHCLK_ADC12;
    PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK2;
    PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
    PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
    PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_SYSCLK;
    PeriphClkInit.USBClockSelection = RCC_USBCLKSOURCE_PLL;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
    {
        HAL::ErrorHandler();
    }
}


uint HAL::GetUID()
{
#ifdef WIN32
    return 123;
#else
    uint8 bytes[12];

    std::memcpy(bytes, (void *)0x1FFFF7E8, 12); //-V566

    return Math::CalculateCRC(bytes, 12);
#endif
}


__attribute((noreturn)) void HAL::ErrorHandler()
{
    while (true)
    {
    }
}
