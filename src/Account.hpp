#pragma once
#include <string>
#include <compare>
#include "CDate.hpp"

// Структура, описывающая один банковский счёт
// Используется в BankDB — таких записей может быть много

struct Account {
    // идентификатор счёта и ФИО
    std::string id, fio;

    // текущий баланс
    float balance{0};

    // процентная ставка
    float rate{0};

    // дата открытия счёта
    CDate open;

    // есть ли у клиента карта
    bool hasCard{false};   

    // Создаёт объект Account из строки CSV
    static Account fromCSV(const std::string& line);

    // Преобразует объект в строку CSV
    std::string toCSV() const;
};