// 2022/05/06 13:37:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define DEF_BUTTON(name, title, keeper, funcOpenClose, funcOnDraw, marked)                                                              \
    static const DButton name = { TypeItem::Button, title, (const Page *)&keeper, funcOpenClose, funcOnDraw,  marked };

#define DEF_STATE(name, title, keeper, funcOpenClose, funcOnDraw, type, is_min)                                                         \
    static const DState name = { TypeItem::State, title, (const Page *)&keeper, funcOpenClose, funcOnDraw, type, is_min};

#define DEF_GOVERNOR(name, title, keeper, beforeOpen, funcAfterDraw, min, max, value)                                                   \
    static const DGovernor name = { TypeItem::Governor, title, (const Page *)&keeper, beforeOpen, funcAfterDraw, min, max, &(value) };

#define DEF_TIMEITEM(name, keeper, beforeOpen, value, state, time)                                                                      \
    static const DTimeItem name = { TypeItem::Time, "", (const Page *)&keeper, beforeOpen, nullptr, &(value), &(state), &(time)};

#define DEF_CHOICE_2(name, title, keeper, beforeOpen, onDraw, cell, name1, name2)                                                       \
    static const DChoice name = { TypeItem::Choice, title, (const Page *)&keeper, beforeOpen, onDraw, &cell, 2, {name1, name2}};

#define DEF_ITEMS_1(name, item)                                                                                     \
    static const Item * const name[] = { (Item *)&item, nullptr };

#define DEF_ITEMS_2(name, item1, item2)                                                                             \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, nullptr };

#define DEF_ITEMS_3(name, item1, item2, item3)                                                                      \
    static const Item * const name[] = { (Item *)&item1,(Item *)&item2, (Item *)&item3, nullptr };

#define DEF_ITEMS_4(name, item1, item2, item3, item4)                                                               \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, (Item *)&item3,                            \
    (Item *)&item4, nullptr };

#define DEF_ITEMS_5(name, item1, item2, item3, item4, item5)                                                        \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, (Item *)&item3,                            \
        (Item *)&item4, (Item *)&item5, nullptr };

#define DEF_ITEMS_6(name, item1, item2, item3, item4, item5, item6)                                                 \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, (Item *)&item3,                            \
        (Item *)&item4, (Item *)&item5, (Item *)&item6, nullptr };

#define DEF_ITEMS_7(name, item1, item2, item3, item4, item5, item6, item7)                                          \
    static const Item * const name[] = { (Item *)&item1, (Item *)&item2, (Item *)&item3,                            \
        (Item *)&item4, (Item *)&item5, (Item *)&item6, (Item *)&item7, nullptr };

#define DEF_PAGE(name, title, keeper, beforeOpen, onDraw, items)                                                    \
    static uint8 ci##name = 0;                                                                                      \
    const DPage name = { TypeItem::Page, title, (const Page *)&keeper, beforeOpen, onDraw, items, &ci##name };

#define DEF_PAGE_1(name, title, keeper, beforeOpen, onDraw, item1)                                                  \
    DEF_ITEMS_1(items##name, item1)                                                                                 \
    DEF_PAGE(name, title, keeper, beforeOpen, onDraw, items##name)

#define DEF_PAGE_2(name, title, keeper, beforeOpen, onDraw, item1, item2)                                                   \
    DEF_ITEMS_2(items##name, item1, item2)                                                                          \
    DEF_PAGE(name, title, keeper, beforeOpen, onDraw, items##name)

#define DEF_PAGE_3(name, title, keeper, beforeOpen, funcOnDraw, item1, item2, item3)                                \
    DEF_ITEMS_3(items##name, item1, item2, item3)                                                                   \
    DEF_PAGE(name, title, keeper, beforeOpen, funcOnDraw, items##name)

#define DEF_PAGE_4(name, title, keeper, beforeOpen, funcOnDraw, item1, item2, item3, item4)                         \
    DEF_ITEMS_4(items##name, item1, item2, item3, item4)                                                            \
    DEF_PAGE(name, title, keeper, beforeOpen, funcOnDraw, items##name)

#define DEF_PAGE_5(name, title, keeper, beforeOpen, funcOnDraw, item1, item2, item3, item4, item5)                  \
    DEF_ITEMS_5(items##name, item1, item2, item3, item4, item5)                                                     \
    DEF_PAGE(name, title, keeper, beforeOpen, funcOnDraw, items##name)

#define DEF_PAGE_6(name, title, keeper, beforeOpen, funcOnDraw, item1, item2, item3, item4, item5, item6)           \
    DEF_ITEMS_6(items##name, item1, item2, item3, item4, item5, item6)                                              \
    DEF_PAGE(name, title, keeper, beforeOpen, funcOnDraw, items##name)

#define DEF_PAGE_7(name, title, keeper, beforeOpen, funcOnDraw, item1, item2, item3, item4, item5, item6, item7)    \
    DEF_ITEMS_7(items##name, item1, item2, item3, item4, item5, item6, item7)                                       \
    DEF_PAGE(name, title, keeper, beforeOpen, funcOnDraw, items##name)
