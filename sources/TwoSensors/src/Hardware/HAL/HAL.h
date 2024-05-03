// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Text/String.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL_PINS.h"


struct PackedTime
{
    uint hours : 5;
    uint minutes : 6;
    uint seconds : 6;
    uint year : 6;           // Год отсчитывается от 2000, т.е. 2023 будет 23
    uint month : 4;
    uint day : 5;
    PackedTime(uint h = 11, uint m = 11, uint s = 11, uint d = 11, uint mo = 11, uint y = 11) :
        hours(h), minutes(m), seconds(s), year(y), month(mo), day(d) {}

    String<> ToString() const;

    uint ToSecs() const;
};


namespace HAL
{
    void Init();

    void Delay(unsigned int timeMS);

    uint GetUID();

    void ErrorHandler();
}


namespace HAL_ADC
{
    void Init();

    float GetVoltage();

    float GetHumidity();

    extern void *handle;           // ADC_HandleTypeDef
}

namespace HAL_I2C1
{
    void Init();

    int8 Read(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Read16(uint8 dev_id, uint8 *data);

    int8 Write(uint8 dev_id, uint8 reg_addr, uint8 *reg_data, uint16 len);
    int8 Write8(uint8 dev_id, uint8 data);
}


namespace HAL_SPI1
{
    void Init();

    void Write(uint8);

    void Write(const void *, int size);

    void WriteRead(const void *out, uint8 *in, int size);
}


namespace HAL_RTC
{
    void Init();

    PackedTime GetTime();

    void SetTime(const PackedTime &);
}


namespace HAL_USART_HI50
{
    void Init(void (*_callback_on_receive)(pchar));

    void Send(uint8);

    void ReceiveCallback(uint8);

    void Update();

    extern void *handle;       // UART_HandleTypeDef
}


namespace HAL_ROM
{
    uint SaveSettings(const Settings *);
    bool LoadSettings(Settings *);
}


namespace HAL_IWDG
{
    void Init();

    void Update();
}


#ifdef __cplusplus
extern "C" {
#endif 

    void NMI_Handler(void);
    void HardFault_Handler(void);
    void MemManage_Handler(void);
    void BusFault_Handler(void);
    void UsageFault_Handler(void);
    void SVC_Handler(void);
    void DebugMon_Handler(void);
    void PendSV_Handler(void);
    void SysTick_Handler(void);
    void USB_LP_CAN_RX0_IRQHandler(void);
    void USART1_IRQHandler(void);
    void USART2_IRQHandler(void);
    void ADC1_2_IRQHandler(void);
    void TIM3_IRQHandler(void);
    void EXTI9_5_IRQHandler(void);

#ifdef __cplusplus
}
#endif
