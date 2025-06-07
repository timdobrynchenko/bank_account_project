#include "CDate.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <ctime> 

/* ---------- parse ---------- */

CDate CDate::parse(const std::string& iso)
{
    unsigned yy, mm, dd;
    char dash1, dash2;
    std::istringstream ss(iso);

    if (!(ss >> yy >> dash1 >> mm >> dash2 >> dd) || dash1 != '-' || dash2 != '-') {
        throw std::runtime_error("bad date format: " + iso);
    }

    // 1. Проверка месяца
    if (mm < 1 || mm > 12)
        throw std::runtime_error("bad month in date: " + iso);

    // 2. Получаем количество дней в месяце (с учётом високосного года)
    auto isLeap = [](unsigned y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    };

    const unsigned daysInMonth[12] = {
        31, isLeap(yy) ? 29u : 28u, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (dd < 1 || dd > daysInMonth[mm - 1])
        throw std::runtime_error("bad day in date: " + iso);

    // 3. Получаем текущий год
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    unsigned currentYear = 1900 + now->tm_year;

    if (yy > currentYear)
        throw std::runtime_error("future year not allowed: " + iso);

    return {yy, mm, dd};
}

/* ---------- str ---------- */
std::string CDate::str() const
{
    std::ostringstream ss;
    ss << std::setfill('0')
       << std::setw(4) << y << '-'
       << std::setw(2) << m << '-'
       << std::setw(2) << d;
    return ss.str();
}

/* ---------- stream operators ---------- */
std::ostream& operator<<(std::ostream& os, const CDate& d)
{
    return os << d.str();
}

std::istream& operator>>(std::istream& is, CDate& d)
{
    std::string tmp;
    if (is >> tmp)
        d = CDate::parse(tmp);
    return is;
}
