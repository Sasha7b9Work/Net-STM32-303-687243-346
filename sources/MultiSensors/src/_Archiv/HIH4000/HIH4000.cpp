// 2023/10/10 16:03:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/HIH4000/HIH4000.h"
#include "Hardware/HAL/HAL.h"


/*                       k1          k2
*   voltage = Vmax * (0.0062 * RH + 0.16), для 25 по Цельсию
* 
*   k2 = Vout0 / 5
*   k1 = (Vout75 - 5 * k2) / 5 / 75
* 
* 
*   true RH = RH * (1.0546 - 0.00216T), T в градусах Цельсия
*/


namespace HIH4000
{
    static const float voltage_supply = 5.0f;
    static const float volt0 = 0.844581f;
    static const float volt75 = 3.088386f;

    static float K2(float voltage_0)
    {
        return voltage_0 / voltage_supply;
    }

    static float K1(float voltage_75, float k2)
    {
        return (voltage_75 - voltage_supply * k2) / voltage_supply / 75.0f;
    }

    static float RH(float voltage, float k1, float k2)
    {
        return (voltage / voltage_supply - k2) / k1;
    }
}


float HIH4000::GetHumidity(float t, float *voltage_out)
{
    float voltage = HAL_ADC::GetHumidity();

    *voltage_out = voltage;

    float k2 = K2(volt0);
    float k1 = K1(volt75, k2);
    float humidity = RH(voltage, k1, k2);

    return humidity / (1.0546f - 0.00216f * t);
}
