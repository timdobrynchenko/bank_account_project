#pragma once
#include <vector>
#include <string>
#include "Account.hpp"
#include "CDate.hpp"

// Класс BankDB — это простая база данных банковских счетов
// Счета хранятся в векторе
// Также запоминается имя последнего открытого файла

class BankDB
{
    // список всех записей (счётов)
    std::vector<Account> data_;

    // путь к текущему открытому CSV-файлу
    std::string currentFile_;

    // Вспомогательные функции для вывода таблицы
    static void printHeader();
    static void printRec(const Account&);

public:
    // загрузить из файла
    void load(const std::string& path);

    // сохранить в файл
    void save(const std::string& path) const;

    // сохранить в тот же файл
    void save() const;

    // вывести все записи на экран
    void show() const;

    // добавить новую запись
    void add();

    // удалить по номеру (начиная с 1)
    void remove(std::size_t oneBasedIdx);

    // отсортировать по ID
    void sortById();

    // найти по ID
    Account* findById(const std::string& id);

    // выбрать по диапазону дат
    void rangeByOpenDate(const CDate& from, const CDate& to) const;

    // найти счета старше 1 года
    void taskProlongation(const CDate& today) const;

    // прогноз баланса на N месяцев
    void taskForecast(int months, const std::string& outFile) const;
};
