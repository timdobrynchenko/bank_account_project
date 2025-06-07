#include "BankDB.hpp"
#include "CDate.hpp"
#include <iostream>

// Вспомогательная функция: безопасно запрашивает у пользователя целое число
// При ошибке ввода очищает поток и сообщает об этом
bool safeInputInt(const std::string& prompt, int& value) {
    std::cout << prompt;
    std::cin >> value;
    if (!std::cin) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Incorrect input, try another value\n";
        return false;
    }
    return true;
}

// Функция запроса даты у пользователя
CDate askDate(const std::string& prompt); 

int main()
{
    BankDB db;

    // Пробуем загрузить базу данных из CSV-файла
    // Если не получилось — просто начинаем с пустой базы
    try {
        db.load("data/accounts.csv");   
    } catch(const std::exception& e) {
        std::cerr << e.what() << "\nStarting with empty DB.\n";
    }

    // Основной цикл: отображаем меню и обрабатываем команды пользователя
    while (true)
    {
        // Меню действий
        std::cout <<
            "\n===== MENU =====\n"
            "1. Show all\n"
            "2. Add record\n"
            "3. Remove record\n"
            "4. Sort by ID\n"
            "5. Find by ID\n"
            "6. Range by date\n"
            "7. Prolongation list (>1 year)\n"
            "8. Forecast balance\n"
            "9. Save (overwrite)\n"
            "10. Save as (new file)\n"
            "0. Exit\n> ";

        int choice;
        if (!safeInputInt("", choice)) continue;

        switch (choice)
        {
        case 1:
            // Показать все записи в базе
            db.show();
            break;

        case 2:
            // Добавить новую запись (данные вводятся вручную)
            db.add();
            break;

        case 3: {
            // Удалить запись с подтверждением
            std::size_t idx;
            if (!safeInputInt("Record number (1-based): ", reinterpret_cast<int&>(idx)))
                continue;
            
            char confirm;
            std::cout << "Delete №" << idx << "? (y/n): ";
            std::cin >> confirm;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(10000, '\n');
                std::cout << "Invalid input, deletion canceled\n";
                continue;
            }
            
            if (confirm == 'y' || confirm == 'Y') {
                db.remove(idx);
            } else {
                std::cout << "Deletion canceled\n";
            }
            break;
        }        

        case 4:
            // Сортировка записей по ID
            db.sortById();
            break;

        case 5: {
            // Поиск по ID
            std::string id;
            std::cout << "ID to find: ";
            std::cin >> id;
            db.findById(id);
            break;
        }

        case 6: {
            // Фильтрация по диапазону дат
            CDate from = askDate("From (YYYY-MM-DD): ");
            CDate to   = askDate("To   (YYYY-MM-DD): ");
            db.rangeByOpenDate(from, to);
            break;
        }

        case 7: {
            // Поиск клиентов, открывших счёт более года назад (для пролонгации)
            CDate today = CDate::today();  
            db.taskProlongation(today);
            break;
        }
        
        case 8: {
            // Прогнозирование баланса через N месяцев
            int m;
            if (!safeInputInt("Months ahead (m): ", m))
                continue;
            std::ostringstream name;
            name << "future_" << m << ".csv";
            db.taskForecast(m, name.str());
            break;
        }

        case 9:
            // Сохранение в исходный файл
            db.save(); 
            break;

        case 10: {
            // Сохранение в новый файл
            std::string out;
            std::cout << "File name: ";
            std::cin  >> out;
            db.save(out);
            break;
        }
            
        case 0:
            // Завершение программы
            std::cout << "Bye!\n";
            return 0;

        default:
            // Обработка некорректного выбора
            std::cout << "Wrong choice.\n";
        }
    }
}
