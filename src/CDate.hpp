#pragma once
/*───────────────────────────────────────────────────────────────────
 *  CDate ‒ минимальная календарная дата (год-месяц-день)
 *   • сравнение и вычитание через std::chrono::sys_days
 *   • parse()  — из строки "YYYY-MM-DD"
 *   • str()    — обратно в строку
 *   • потоковые операторы << и >>
 *   • constexpr где это возможно
 *──────────────────────────────────────────────────────────────────*/
#include <chrono>    // year_month_day, sys_days
#include <compare>   // <=>   (C++20)
#include <string>
#include <iosfwd>    // объявление ostream/istream

struct CDate
{
    unsigned y{}, m{}, d{};                      // например 2025-06-07

    constexpr CDate() = default;
    constexpr CDate(unsigned yy, unsigned mm, unsigned dd)
        : y(yy), m(mm), d(dd) {}

private:
    /* helper: переводим в chrono-дату для всех операций */
    std::chrono::sys_days toChrono() const
    {
        using namespace std::chrono;
        return year{ int(y) } / month{ m } / day{ d };
    }

public:
    /* ---------- сравнение (operator<=> генерирует всё остальное) ---------- */
    constexpr auto operator<=>(const CDate& rhs) const
    { return toChrono() <=> rhs.toChrono(); }

    constexpr bool operator==(const CDate&) const = default;

    /* ---------- разница дней ---------- */
    long operator-(const CDate& rhs) const
    {
        using namespace std::chrono;
        return (toChrono() - rhs.toChrono()).count();   // days.count()
    }

    /* ---------- строковый ввод/вывод ---------- */
    static CDate   parse(const std::string& iso);  // "YYYY-MM-DD"
    std::string    str() const;                    // → строка

};

/* потоковые операторы (реализованы в .cpp) */
std::ostream& operator<<(std::ostream&, const CDate&);
std::istream& operator>>(std::istream&, CDate&);