#include "BankDB.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <sstream>
#include <unordered_set>
#include <stdexcept>
#include <regex> 
#include <unordered_set>
#include <cmath>

// Запрашивает у пользователя строку и пытается преобразовать в дату
CDate askDate(const std::string& prompt)
{
    std::string s;
    while (true) {
        std::cout << prompt;
        std::cin  >> s;
        try { return CDate::parse(s); }
        catch (...) { std::cout << "  bad date; try again.\n"; }
    }
}

// Проверяет, соответствует ли ID формату: 3 буквы + 4 цифры
static bool isValidId(const std::string& s)
{
    static const std::regex re{R"([A-Za-z]{3}\d{4})"};
    return std::regex_match(s, re);
}

// Проверяет и парсит строку в баланс (дробное число)
static bool parseBalance(const std::string& s, float& out)
{
    static const std::regex re{R"(\d+(\.\d{1,2})?)"};
    if (!std::regex_match(s, re)) return false;
    out = std::stof(s);
    return true;
}

// Выводит заголовок таблицы
void BankDB::printHeader()
{
    std::cout << std::left
        << std::setw(10) << "ID"
        << std::setw(45) << "FIO"
        << std::setw(20) << "Balance"
        << std::setw(7)  << "%"
        << std::setw(12) << "Open"
        << "Card\n";
}

// Выводит одну запись
void BankDB::printRec(const Account& a)
{
    std::ios oldState(nullptr);
    oldState.copyfmt(std::cout);

    std::cout << std::setw(10) << a.id
              << std::setw(45) << a.fio
              << std::setw(20) << std::fixed << std::setprecision(2) << a.balance
              << std::setw(7)  << a.rate
              << std::setw(12) << a.open.str()
              << (a.hasCard ? "yes" : "no") << '\n';
    
    std::cout.copyfmt(oldState);
}

// Загружает базу из CSV-файла
void BankDB::load(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("Can't open " + path);

    currentFile_ = path;
    data_.clear();
    std::string line;
    while (std::getline(in, line))
        if (!line.empty())
            data_.push_back(Account::fromCSV(line));

    std::cout << "Loaded " << data_.size() << " records from " << path << '\n';
}

// Сохраняет базу в указанный файл
void BankDB::save(const std::string& path) const
{
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("Can't open " + path + " for write");

    for (const auto& a : data_) out << a.toCSV() << '\n';
    std::cout << "Saved to " << path << '\n';
}

// Сохраняет базу в текущий файл (тот, что был открыт)
void BankDB::save() const
{
    if (currentFile_.empty()) {
        std::cout << "No file loaded yet. Use Save As.\n";
        return;
    }
    save(currentFile_);
}

// Выводит все записи
void BankDB::show() const
{
    if (data_.empty()) { std::cout << "<empty>\n"; return; }
    printHeader();
    for (const auto& a : data_) printRec(a);
}

// Добавляет новую запись в базу
void BankDB::add()
{
    Account a;
    std::string tmp;

    // Ввод ID
    while (true) {
        std::cout << "ID (3 letters & 4 digits, example - ACC0001): ";
        std::cin  >> tmp;
        if (isValidId(tmp)) { a.id = tmp; break; }
        std::cout << "  wrong format. Must be 3 letters + 4 digits.\n";
    }

    // Ввод ФИО
    std::cout << "Surname Name Patronymic (max 40 chars): ";
    std::getline(std::cin >> std::ws, a.fio);
    if (a.fio.size() > 40) a.fio.resize(40);   // обрезаем лишнее

    // Ввод баланса
    while (true) {
        std::cout << "Balance (exmple 12345.67, max 12 chars): ";
        std::cin  >> tmp;
        if (parseBalance(tmp, a.balance)) break;
        std::cout << "Enter non-negative number, max 2 decimals\n";
    }

    // Ввод процентной ставки
    while (true) {
        std::cout << "Rate % (e.g. 4.5): ";
        std::cin >> a.rate;
        if (!std::cin.fail()) break;
    
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input. Use format like 4.5\n";
    }
    

    // Ввод даты открытия
    a.open = askDate("Open date (YYYY-MM-DD): ");

    // Ввод флага "есть карта"
    char card;
    while (true) {
        std::cout << "Has card? (y/n): ";
        std::cin >> card;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Use y or n\n";
            continue;
        }

        // Проверяем именно символ (не просто успешный ввод)
        if (card == 'y' || card == 'Y' || card == 'n' || card == 'N')
            break;

        std::cout << "Invalid input. Please enter y or n\n";
    }
    a.hasCard = (card == 'y' || card == 'Y');
    data_.push_back(std::move(a));
    std::cout << "Added\n";
}

// Удаляет запись по индексу (нумерация с 1)
void BankDB::remove(std::size_t oneBasedIdx)
{
    if (oneBasedIdx == 0 || oneBasedIdx > data_.size()) {
        std::cout << "Bad index.\n";
        return;
    }
    data_.erase(data_.begin() + (oneBasedIdx - 1));
    std::cout << "Removed.\n";
}

// Сортировка записей по ID
void BankDB::sortById()
{
    std::sort(data_.begin(), data_.end(),
                [](const Account& a, const Account& b) { 
                    return a.id < b.id; 
                });
    std::cout << "Sorted.\n";
}

// Поиск записи по ID
Account* BankDB::findById(const std::string& id)
{
    auto it = std::find_if(data_.begin(), data_.end(),
                [&](const Account& a) { 
                    return a.id == id; 
                });
    if (it == data_.end()) {
        std::cout << "Not found.\n";
        return nullptr;
    }
    printHeader();
    printRec(*it);
    return &*it;
}

// Фильтр записей по дате открытия
void BankDB::rangeByOpenDate(const CDate& from, const CDate& to) const
{
    printHeader();
    for (const auto& a : data_)
        if (from <= a.open && a.open <= to)
            printRec(a);
}

// Выводит список ФИО, чьи счета открыты более года назад
void BankDB::taskProlongation(const CDate& today) const
{
    constexpr long ONE_YEAR = 365;
    std::unordered_set<std::string> uniq;

    for (const auto& acc : data_)
        if ((today - acc.open) > ONE_YEAR)
            uniq.insert(acc.fio);

    std::cout << "\n=== Clients to notify (" << uniq.size() << ") ===\n";
    if (uniq.empty()) return;
    for (const auto& fio : uniq) std::cout << fio << '\n';
}

// Делает прогноз баланса на N месяцев вперёд и сохраняет в файл
void BankDB::taskForecast(int m, const std::string& outFile) const
{
    if (m <= 0) { std::cout << "m must be > 0\n"; return; }

    std::ofstream out(outFile, std::ios::binary);
    if (!out) { std::cerr << "Can't open " << outFile << '\n'; return; }

    double k;
    for (auto acc : data_) {
        k = std::pow(1.0 + acc.rate / 100.0 / 12.0, m);
        acc.balance = static_cast<float>(acc.balance * k);
        out << acc.toCSV() << '\n';
    }
    std::cout << "Forecast saved to " << outFile << '\n';
}