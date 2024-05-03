// 2022/04/20 08:53:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Modules/ST7735/ST7735.h"
#include "Display/Font/Font.h"
#include "Utils/Text/String.h"
#include "Display/Zones.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Storage/Measures.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Utils/Math.h"
#include <cmath>


namespace Display
{
    struct DMeasure
    {
        String<> str_value;

        Measure::E name;
        Measure value;              // Последнее установленное значение
        int position;               // Текущая отрисовываемая позиция
        uint time;                  // Время последнего изменения текущей отрисовываемой позиции

        DMeasure(Measure::E n) : name(n), position(0), time(0) {}

        void Draw(const int x, const int y, int size = 1);

        String<> Name();

        String<> Units();
    };

    static DMeasure measures[Measure::Count] =
    {
        DMeasure(Measure::Count),
        DMeasure(Measure::Pressure),
        DMeasure(Measure::Humidity),
        DMeasure(Measure::DewPoint),
        DMeasure(Measure::Velocity),
        DMeasure(Measure::Latitude),
        DMeasure(Measure::Longitude),
        DMeasure(Measure::Altitude),
        DMeasure(Measure::Azimuth),
        DMeasure(Measure::Illuminate),
        DMeasure(Measure::Distance)
    };

    namespace Buffer
    {
        static uint8 buffer[WIDTH * HEIGHT / 2];       // Четырёхбитный цвет

        uint8 GetPixels(int x, int y)
        {
            return buffer[(y * WIDTH + x) / 2];
        }

        uint8 *GetLine(int x, int y)
        {
            return &buffer[(y * WIDTH + x) / 2];
        }

        static void SetPoint(int x, int y)
        {
            if (x < 0) { return; }
            if (y < 0) { return; }
            if (x >= WIDTH) { return; }
            if (y >= HEIGHT) { return; }

            uint8 *pixels = &buffer[(y * WIDTH + x) / 2];

            uint8 value = *pixels;

            if (x & 0x1)
            {
                value &= 0x0F;
                value |= (Color::GetCurrent() << 4);
            }
            else
            {
                value &= 0xF0;
                value |= Color::GetCurrent();
            }

            *pixels = value;
        }

        static void DrawHLine(int y, int x1, int x2)
        {
            if ((x1 % 2) == 0 && (x2 % 2) == 0)
            {
                uint8 *first_byte = &buffer[y * Display::WIDTH / 2 + x1 / 2];

                uint8 value = (uint8)((uint8)(Color::GetCurrent()) + (Color::GetCurrent() << 4));

                std::memset(first_byte, value, (uint)(x2 - x1) / 2U);
            }
            else
            {
                for (int x = x1; x < x2; x++)
                {
                    Display::Buffer::SetPoint(x, y);
                }
            }
        }

        static void Fill(Color::E color)
        {
            uint8 value = (uint8)((int)(color) | (int)(color << 4));

            std::memset(buffer, value, WIDTH * HEIGHT / 2);
        }
    }
}


namespace Display
{
    bool need_redraw = true;

    static bool mode_compass = false;        // Если true - рисуем компас

    static ZoneFPS zoneFPS;

    static Zone *zones[] =
    {
        &zoneFPS,
        nullptr
    };

    void DrawZones();

    static void DrawMeasures(uint timeMS);

    static void DrawStar();
}


void HLine::Draw(int x0, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::DrawHLine(y, x0, x0 + width);
}


void Line::Rotate(int x0, int y0, float angle_rad)
{
    float cos = std::cosf(angle_rad);
    float sin = std::sinf(angle_rad);

    Rotate(&x1, &y1, x0, y0, cos, sin);

    Rotate(&x2, &y2, x0, y0, cos, sin);
}


void Line::Rotate(float *x_in_out, float *y_in_out, int x0, int y0, float cos, float sin)
{
    float dx = *x_in_out - (float)x0;
    float dy = *y_in_out - (float)y0;

    float x = dx * cos - dy * sin + (float)x0;

    float y = dx * sin + dy * cos + (float)y0;

    *x_in_out = x;
    *y_in_out = y;
}


void Line::Draw(Color::E color)
{
    Color::SetCurrent(color);

    if ((x2 - x1) == 0 && (y2 - y1) == 0)
    {
        ++x1;
    }
    int x = (int)(x1);
    int y = (int)(y1);
    int dx = (int)std::fabsf((float)(x2 - x1));
    int dy = (int)std::fabsf((float)(y2 - y1));
    int s1 = Math::Sign((int)(x2 - x1));
    int s2 = Math::Sign((int)(y2 - y1));
    int temp;
    int exchange = 0;
    if (dy > dx)
    {
        temp = dx;
        dx = dy;
        dy = temp;
        exchange = 1;
    }
    int e = 2 * dy - dx;
    int i = 0;
    for (; i <= dx; i++)
    {
        Point().Set(x, y);

        while (e >= 0)
        {
            if (exchange)
            {
                x += s1;
            }
            else
            {
                y += s2;
            }
            e = e - 2 * dx;
        }
        if (exchange)
        {
            y += s2;
        }
        else
        {
            x += s1;
        }
        e = e + 2 * dy;
    }
}


void VLine::Draw(int x, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int y = y0; y < y0 + height; y++)
    {
        Display::Buffer::SetPoint(x, y);
    }
}


void Point::Set(int x, int y, Color::E color)
{
    Color::SetCurrent(color);

    Display::Buffer::SetPoint(x, y);
}


void Rectangle::Fill(int x0, int y0, Color::E color)
{
    Color::SetCurrent(color);

    for (int x = x0; x < x0 + width; x++)
    {
        VLine(height).Draw(x, y0);
    }
}


void Rectangle::DrawFilled(int x, int y, Color::E fill, Color::E draw)
{
    Rectangle(width, height).Draw(x, y, draw);

    Rectangle(width - 2, height - 2).Fill(x + 1, y + 1, fill);
}


void Circle::Draw(int _x, int _y, Color::E color)
{
    Color::SetCurrent(color);

    int x = radius;
    int y = 0;
    int radiusError = 1 - x;

    int dx = (_x - radius);
    int dy = (_y - radius);

    while (x >= y)
    {
        Display::Buffer::SetPoint(x + radius + dx, y + radius + dy);
        Display::Buffer::SetPoint(y + radius + dx, x + radius + dy);
        Display::Buffer::SetPoint(-x + radius + dx, y + radius + dy);
        Display::Buffer::SetPoint(-y + radius + dx, x + radius + dy);
        Display::Buffer::SetPoint(-x + radius + dx, -y + radius + dy);
        Display::Buffer::SetPoint(-y + radius + dx, -x + radius + dy);
        Display::Buffer::SetPoint(x + radius + dx, -y + radius + dy);
        Display::Buffer::SetPoint(y + radius + dx, -x + radius + dy);
        y++;
        if(radiusError < 0)
        {
            radiusError += 2 * y + 1;
        }
        else
        {
            x--;
            radiusError += 2 * (y - x) + 1;
        }
    }
}


void Rectangle::Draw(int x, int y, Color::E color)
{
    HLine(width).Draw(x, y, color);
    HLine(width).Draw(x, y + height - 1);
    VLine(height).Draw(x, y);
    VLine(height).Draw(x + width - 1, y);
}


void Display::SetMeasure(const Measure &measure, uint timeMS)
{
    DMeasure &value = measures[measure.GetName()];

    if (value.value.GetDouble() == measure.GetDouble()) //-V550
    {
        if (value.str_value.Size())
        {
            return;
        }
    }

    if (!Beeper::IsRunning())
    {
        Settings::SaveMeasure(measure);
    }

    value.position = 0;
    value.time = timeMS;
    value.value = measure;

    pchar format = "%.1f";

    if (value.name == Measure::Longitude || value.name == Measure::Latitude)
    {
        format = "%.7f";
    }

    value.str_value.SetFormat(format, (double)measure.GetDouble());
//    value.current[6] = '\0';
}


void Display::DMeasure::Draw(const int x0, const int y0, int size)
{
    int width_zone = 38;
    int height_zone = 12;
    int y_zone = y0;

    Rectangle(width_zone, height_zone).Fill(x0, y_zone, Color::BLACK);

    Font::Text::DrawBig(x0, y0, size, str_value.c_str(), Color::WHITE);
}


void Display::DrawZones()
{
    for (int i = 0; true; i++)
    {
        Zone *zone = zones[i];

        if (zone == nullptr)
        {
            break;
        }

        zone->Draw();
    }
}


void Display::BeginScene(Color::E color)
{
    Buffer::Fill(color);
}


void Display::EndScene()
{
    DrawStar();

    ST7735::WriteBuffer(0, 0, WIDTH, HEIGHT);
}


void Display::Update(uint timeMS)
{
    BeginScene(Color::BLACK);

    DrawMeasures(timeMS);

    EndScene();
}


void Display::DrawMeasures(uint)
{
    Font::Set(TypeFont::_12_10);

    const int x0 = 3;

    // Пустое место между строками
    const int d_lines = (Display::HEIGHT - 5 * Font::Height()) / (5 + 1);

    const int y0 = d_lines;
    const int dY = d_lines + Font::Height();

    static const Measure::E names[Measure::Count] =
    {
        Measure::Temperature,
        Measure::Pressure,
        Measure::Humidity,
        Measure::DewPoint,
        Measure::Velocity,
        Measure::Latitude,
        Measure::Longitude,
        Measure::Altitude,
        Measure::Azimuth,
        Measure::Illuminate,
        Measure::Distance
    };

    int y = y0;

    for (int i = 0; i < Measure::Count; i++)
    {
        DMeasure &measure = measures[names[i]];

        if (measure.str_value.Size())
        {
            int x = 93;

            int width = 30;
            int height = 15;

            if (measure.name > Measure::Velocity)
            {
                if (measure.value.GetDouble() < 10.0)
                {
                    x += 14;
                }
                else if (measure.value.GetDouble() < 100.0)
                {
                    x += 8;
                }
            }

            String<>("%s", measure.Name().c_str()).Draw(x0, y, measure.value.InRange() ? Color::WHITE : Color::FLASH_10);
            measure.Units().Draw(134, y, Color::WHITE);
            measure.Draw(x, y);

            ST7735::WriteBuffer(x - 1, y, width, height);
            y += dY;
        }
    }
}


void Display::DrawStar()
{
    if (((TIME_MS / 500) % 2) == 0)
    {
        String<>("*").Draw(156, 0, Color::WHITE);
    }
}


String<> Display::DMeasure::Name()
{
    static const pchar names[Measure::Count] =
    {
        "ТЕМПЕРАТУРА",
        "ДАВЛЕНИЕ",
        "ВЛАЖНОСТЬ",
        "ТОЧКА РОСЫ",
        "СКОРОСТЬ",
        "ШИРОТА",
        "ДОЛГОТА",
        "ВЫСОТА",
        "АЗИМУТ",
        "ОСВЕЩЁННОСТЬ",
        "ДИСТАНЦИЯ"
    };

    String<> result(names[name]);

    return result;
}

String<> Display::DMeasure::Units()
{
    static const pchar units[Measure::Count] =
    {
        "ЁС",
        "гПа",
        "%%",
        "ЁС",
        "м/с",
        "Ё",
        "Ё",
        "м",
        "Ё",
        "лк",
        "м"
    };

    return String<>(units[name]);
}


void Display::Mode::EnableCompass(bool enable)
{
    mode_compass = enable;
}


bool Display::Mode::IsEnabledCompass()
{
    return mode_compass;
}
