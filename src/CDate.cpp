#include "CDate.hpp"
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <ctime> 

// Преобразует строку "YYYY-MM-DD" в CDate
CDate CDate::parse(const std::string& iso)
{
    unsigned yy, mm, dd;
    char dash1, dash2;
    std::istringstream ss(iso);

    // Проверяем, что строка состоит из года, месяца и дня, разделённых дефисами
    if (!(ss >> yy >> dash1 >> mm >> dash2 >> dd) || dash1 != '-' || dash2 != '-') {
        throw std::runtime_error("bad date format: " + iso);
    }

    // Проверяем, что месяц в пределах от 1 до 12
    if (mm < 1 || mm > 12)
        throw std::runtime_error("bad month in date: " + iso);

    // Проверяем число в зависимости от месяца и високосности года
    auto isLeap = [](unsigned y) {
        return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    };

    const unsigned daysInMonth[12] = {
        31, isLeap(yy) ? 29u : 28u, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (dd < 1 || dd > daysInMonth[mm - 1])
        throw std::runtime_error("bad day in date: " + iso);

    // Не разрешаем ввод будущего года
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    unsigned currentYear = 1900 + now->tm_year;

    if (yy > currentYear)
        throw std::runtime_error("future year not allowed: " + iso);

    return {yy, mm, dd};
}

// Преобразует дату в строку "YYYY-MM-DD"
std::string CDate::str() const
{
    std::ostringstream ss;
    ss << std::setfill('0')
       << std::setw(4) << y << '-'
       << std::setw(2) << m << '-'
       << std::setw(2) << d;
    return ss.str();
}

// Вывод даты в поток (std::cout << date)
std::ostream& operator<<(std::ostream& os, const CDate& d)
{
    return os << d.str();
}

// Считывание даты из потока (std::cin >> date)
std::istream& operator>>(std::istream& is, CDate& d)
{
    std::string tmp;
    if (is >> tmp)
        d = CDate::parse(tmp);
    return is;
}

// Возвращает текущую дату с компьютера
CDate CDate::today()
{
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    return {
        static_cast<unsigned short>(now->tm_year + 1900),
        static_cast<unsigned short>(now->tm_mon + 1),
        static_cast<unsigned short>(now->tm_mday)
    };
}
