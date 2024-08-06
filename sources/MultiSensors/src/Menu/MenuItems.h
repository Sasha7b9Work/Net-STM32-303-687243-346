// 2022/05/05 12:27:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Display.h"
#include "Hardware/Keyboard.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


struct TypeItem
{
    enum E
    {
        Page,           // —траница содержит другие элементы управлени€
        Choice,         // ¬ыбор из нескольких значений
        Button,         //  нопка - по нажатию что-то происходит
        Governor,       // ћожно измен€ть значение в некоторых пределах
        Time,
        State,          // ќтображает состо€ние измерени€ - величина, и врем€ сохранени€
        Count
    };
};


struct Page;
struct Choice;
struct Button;
struct Governor;
struct TimeItem;
struct StateItem;


typedef void(*FuncOpenClose)(bool);
typedef void(*FuncAfterDraw)(int, int, Color::E fill, Color::E draw);


#define COMMON_PART_ITEM    TypeItem::E    type;             \
                            pchar          title;            \
                            const Page    *keeper;           \
                            FuncOpenClose  funcOnOpenClose;  \
                            FuncAfterDraw  funcOnDraw

struct DItem
{
    COMMON_PART_ITEM;
};


struct Item
{
    static const int WIDTH = Display::WIDTH;
    static const int HEIGHT = 20;

    String<> Title() const;

    void Draw(int x, int y, bool active) const;

    void DrawOpened(int x, int y, bool active) const;
    void DrawClosed(int x, int y, bool active) const;

    Color::E ColorFill() const;
    Color::E ColorDraw() const;

    void ShortPressure(const Key &) const;
    void LongPressure(const Key &) const;
    void DoubleClick() const;

    const Page *Keeper() const { return ToDItem()->keeper; }

    void Open() const;
    void Close() const;

    bool IsOpened() const;

    // ¬озвращает пор€дковый номер на странице
    int NumberOnPage() const;

    static const Item *Opened() { return opened_item; }

    bool IsPage() const     { return (ToDItem()->type == TypeItem::Page);   }
    bool IsChoice() const   { return (ToDItem()->type == TypeItem::Choice); }
    bool IsButton() const   { return (ToDItem()->type == TypeItem::Button); }
    bool IsGovernor() const { return (ToDItem()->type == TypeItem::Governor); }
    bool IsTimeItem() const { return (ToDItem()->type == TypeItem::Time); }

    const DItem *ToDItem() const { return (DItem *)this; }

    const Page *ToPage() const { return (const Page *)this; }
    const Choice *ToChoice() const { return (const Choice *)this; }
    const Button *ToButton() const { return (const Button *)this; }
    const Governor *ToGovernor() const { return (const Governor *)this; }
    const TimeItem *ToTimeItem() const { return (const TimeItem *)this; }
    const StateItem *ToStateItem() const { return (const StateItem *)this; }

    static Item Empty;

protected:

    virtual ~Item() { }

    static const Item *opened_item;
};


//-------------------------------------------------------------------------------------------------


struct DPage
{
    COMMON_PART_ITEM;

    const Item *const *items;
    uint8 *currentItem;
};


struct Page : public Item
{
    static const int NUM_ITEMS_ON_SCREEN = 5;

    void DrawOpened(int x, int y, bool acitve) const;
    void DrawClosed(int x, int y, bool active) const;

    void ShortPressure(const Key &) const;
    void LongPressure(const Key &) const;
    void DoubleClick() const;

    void Close() const;

    const DPage *ToDPage() const { return (DPage *)this; }

    const Item *CurrentItem() const;

private:

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y, bool active) const;

    // ѕервый выводимый итем на текущей странице
    int FirstItemOnScreen() const;

    // ѕоследний выводимый итем на текущей странице
    int LastItemOnScreen() const;

    int NumItems() const;

    virtual ~Page() override { }
};


//-------------------------------------------------------------------------------------------------


struct DChoice
{
    COMMON_PART_ITEM;

    uint8 *const cell;
    const uint8 count;
    pchar names[2];
};


struct Choice : public Item
{
    void DrawClosed(int x, int y, bool active) const;
    void DrawOpened(int x, int y, bool active) const;

    void ShortPressure(const Key &) const;
    void LongPressure() const;
    void DoubleClick() const;

    pchar CurrentName() const;

    const DChoice *ToDChoice() const { return (DChoice *)this; }
};


//-------------------------------------------------------------------------------------------------


struct DButton
{
    COMMON_PART_ITEM;

    bool *marked;            // ≈сли это значение равно true, то кнопка с галочкой. “ипа "выбрано"
};


struct Button : public Item
{
    void ShortPressure(const Key &) const;
    void LongPressure() const { if (ToDItem()->funcOnOpenClose) { ToDItem()->funcOnOpenClose(true); } }
    void DoubleClick() const;

    void DrawClosed(int x, int y, bool active) const;
    void DrawOpened(int x, int y, bool active) const;

    const DButton *ToDButton() const { return (DButton *)this; }
};


//-------------------------------------------------------------------------------------------------


struct DState
{
    COMMON_PART_ITEM;

    Measure::E type_meas;

    const bool is_min;          // ≈сли true, то хранит минимальное значение, иначе - максимальное
};


struct StateItem : public Item
{
    void ShortPressure(const Key &) const;
    void LongPressure(const Key &) const;
    void DoubleClick() const;

    void DrawClosed(int x, int y, bool active) const;
    void DrawOpened(int x, int y, bool active) const;

    const DState *ToDState() const { return (DState *)this; } //-V1027
};


//-------------------------------------------------------------------------------------------------


struct DGovernor
{
    COMMON_PART_ITEM;

    int min;
    int max;
    int *value;
};


struct Governor : public Item
{
    void DrawClosed(int x, int y, bool active) const;
    void DrawOpened(int x, int y, bool active) const;

    void ShortPressure(const Key &) const;
    void LongPressure() const;
    void DoubleClick() const;

    const DGovernor *ToDGovernor() const { return (DGovernor *)this; }

private:
    void DrawControls(int x, int y) const;

    void DrawControl(int x, int y, const String<> &, bool active) const;

    struct ActiveControl
    {
        enum E
        {
            Increase,       // ѕри коротком нажатии увеличение значени€
            Decrease,       // ѕри коротком нажатии уменьшение значени€
            Close,          // ѕри коротком нажатии закрываетс€
            Count
        };
    };

    static ActiveControl::E active_control;
};


//-------------------------------------------------------------------------------------------------


struct DTimeItem
{
    COMMON_PART_ITEM;

    int *cur_field;     // ќбласть, в которой находитс€ курсор
                        // [0...5] <-> [часы ... год], 6 - применить, 7 - выход
    int *state;         // 0 - выбор пол€ дл€ изменени€
                        // 1 - изменение
    PackedTime *time;
};


struct TimeItem : public Item
{
    void DrawClosed(int x, int y, bool active) const;
    void DrawOpened(int x, int y, bool active) const;

    void ShortPressure(const Key &) const;
    void LongPressure(const Key &) const;
    void DoubleClick() const;

    const DTimeItem *ToDTimeItem() const { return (DTimeItem *)this; }
private:

    void ChangeCurrentField(const Key &) const;
};
