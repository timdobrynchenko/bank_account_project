#pragma once // Избегаем повторного вызова заголовка
#include <iostream>

struct CDate {
    unsigned short year, month, day;
    void input();
    void print() const;
    bool opearator<(const CDate& other) const;
    bool isOdlerThanYearFrom(const CDate& current) const;
}