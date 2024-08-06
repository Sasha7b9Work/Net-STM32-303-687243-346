// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/BME280/BME280.h"
#include "Modules/BME280/bme280_driver.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <stm32f3xx_hal.h>
#include <cstring>
#include <cstdlib>
#include <cmath>


namespace BME280
{
    static bme280_dev dev;

    static unsigned int timeNext = 1;       // Время следующего измерения

    // Попытка соединения с усройством по адресу id
    static bool AttemptConnection(uint8 id);

    static float CalculateDewPoint(float temperature, float humidity);

    static float CalculateF(float temperature, float humidity);
}


bool BME280::Init()
{
    if (AttemptConnection(BME280_I2C_ADDR_PRIM))
    {
        return true;
    }

    if (AttemptConnection(BME280_I2C_ADDR_SEC))
    {
        return true;
    }

    return false;
}


bool BME280::AttemptConnection(uint8 id)
{
    dev.dev_id = id;
    dev.intf = BME280_I2C_INTF;
    dev.read = HAL_I2C1::Read;
    dev.write = HAL_I2C1::Write;
    dev.delay_ms = HAL_Delay;

    bme280_init(&dev);

    uint8_t settings_sel;

    dev.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev.settings.filter = BME280_FILTER_COEFF_16;
    dev.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;

    bme280_set_sensor_settings(settings_sel, &dev);

    bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev);

    /* Delay while the sensor completes a measurement */
    dev.delay_ms(70);

    uint8 chip_id = 0x00;

    bme280_get_regs(0xD0, &chip_id, 1, &dev);

    return (chip_id == 0x56) || (chip_id == 0x57) || (chip_id == 0x58) || (chip_id == 0x60);
}


float BME280::CalculateDewPoint(float temperature, float humidity)
{
    float f = CalculateF(temperature, humidity);

    return (237.7f * f) / (17.27f - f);
}


float BME280::CalculateF(float temperature, float humidity)
{
    return (17.27f * temperature) / (237.7f + temperature) + std::log(humidity / 100.0f);
}


bool BME280::GetMeasures(Measure *temp, Measure *pressure, Measure *humidity, Measure *dew_point)
{
    temp->Clear();
    pressure->Clear();
    humidity->Clear();
    dew_point->Clear();

    if(HAL_GetTick() < timeNext)
    {
        return false;
    }

    timeNext += TIME_MEASURE + (uint)(std::rand() % 100);

#ifdef IN_MODE_TEST

    static float value = 1.1f;

    value *= 7.1f;
    temp->Set(Measure::Count, value / 100.0f);

    value *= 1.2f;
    pressure->Set(Measure::Pressure, value / 100.0f);

    value *= 0.83f;
    humidity->Set(Measure::Humidity, value / 99.28f);

    if (value > 1e4f)
    {
        value = 1.34f;
    }

    dew_point->Set(Measure::DewPoint, CalculateDewPoint((float)temp->GetDouble(), (float)humidity->GetDouble()));

    return true;

#else

    bme280_data comp_data;

    int8 result = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev);

    if (result == BME280_OK)
    {
        temp->Set(Measure::Count, comp_data.temperature);
        pressure->Set(Measure::Pressure, comp_data.pressure / 100.0);
        humidity->Set(Measure::Humidity, comp_data.humidity);
        dew_point->Set(Measure::DewPoint, CalculateDewPoint((float)temp->GetDouble(), (float)humidity->GetDouble()));
    }

    return (result == BME280_OK);

#endif
}
