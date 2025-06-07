#pragma once
#include <chrono>
#include <compare>
#include <string>
#include <iosfwd>

// Класс даты: год, месяц, день
// Используется для сравнения, вычитания и преобразования в строку

struct CDate
{
    // Год, месяц, день
    unsigned y{}, m{}, d{};

    constexpr CDate() = default;
    constexpr CDate(unsigned yy, unsigned mm, unsigned dd)
        : y(yy), m(mm), d(dd) {}

private:
    // Вспомогательная функция: преобразует дату в chrono-формат
    std::chrono::sys_days toChrono() const
    {
        using namespace std::chrono;
        return year{ int(y) } / month{ m } / day{ d };
    }

public:
    // Сравнение дат (<, >, == и т.д.)
    constexpr auto operator<=>(const CDate& rhs) const
    { return toChrono() <=> rhs.toChrono(); }

    constexpr bool operator==(const CDate&) const = default;

    // Разница между датами в днях
    long operator-(const CDate& rhs) const
    {
        using namespace std::chrono;
        return (toChrono() - rhs.toChrono()).count();   // days.count()
    }

    // Строка → дата (формат "YYYY-MM-DD")
    static CDate parse(const std::string& iso);

    // Дата → строка
    std::string str() const;

    // Текущая системная дата
    static CDate today();
};

// Поддержка std::cout << date и std::cin >> date
std::ostream& operator<<(std::ostream&, const CDate&);
std::istream& operator>>(std::istream&, CDate&);