#pragma once
#include <vector>
#include <string>
#include "Account.hpp"
#include "CDate.hpp"

/*--------------------------------------------------------------
 *  BankDB — простая «база» вектором Account
 *-------------------------------------------------------------*/
class BankDB
{
    std::vector<Account> data_;                 // все записи
    std::string currentFile_;          // ← хранит последний открытый CSV

    static void printHeader();                  // ↓ вспомогательный вывод
    static void printRec(const Account&);

public:
    /* Этап 3: CSV I/O */
    void load(const std::string& path);         // чтение CSV
    void save(const std::string& path) const;   // запись CSV
    void save() const;                          // NEW: перезаписать currentFile_
    void show() const;                          // вывести таблицу

    /* Этап 4: базовые операции */
    void add();                                 // добавить счёт
    void remove(std::size_t oneBasedIdx);       // удалить (номера с 1)
    void sortById();                            // сортировка по id
    Account* findById(const std::string& id);   // поиск
    void rangeByOpenDate(const CDate& from,
                         const CDate& to) const; // фильтр

    /* Этап 5: мои задачи из варианта 11 */
    void taskProlongation(const CDate& today) const;
    void taskForecast(int months, const std::string& outFile) const;
};
