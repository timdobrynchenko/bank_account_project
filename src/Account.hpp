#pragma once
#include <string>
#include <compare>
#include "CDate.hpp"

struct Account {
    std::string id, fio;
    float balance{0};
    float rate{0};
    CDate open;
    bool hasCard{false};   

    static Account fromCSV(const std::string& line);
    std::string toCSV() const;
};