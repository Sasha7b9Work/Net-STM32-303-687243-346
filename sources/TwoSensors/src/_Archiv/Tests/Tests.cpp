// 2023/12/12 08:15:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Tests/Tests.h"
#include "Settings/Settings.h"
#include <cstdlib>


namespace Test
{
    namespace Settings
    {
        static void FillRandom(::Settings &set)
        {
            for (int i = 0; i < Measure::Count; i++)
            {
                set.display.show_measure[i] = (uint8)std::rand();
            }

            set.display.typeDisplaydInfo.value = (TypeDisplayedInformation::E)std::rand();

            set.system.serial_number = (int)std::rand();

            for (int i = 0; i < NUM_MEASURES_TO_CONTROL; i++)
            {
                set.measures.limit_min[i] = (int)std::rand();
                set.measures.limit_max[i] = (int)std::rand();
                set.measures.value_min[i] = (float)std::rand() * 1.123423f;
                set.measures.value_max[i] = (float)std::rand() * 3.235235f;
            }
        }

        bool Run()
        {
            ::Settings settings_save;

            for (int i = 0; i < 10; i++)
            {
                FillRandom(settings_save);

//                ::Settings::Save(&settings_save);

                ::Settings settings_load;

//                ::Settings::Load(&settings_load);

                if (settings_load != settings_save)
                {
                    return false;
                }
            }

            return true;
        }
    }
}
