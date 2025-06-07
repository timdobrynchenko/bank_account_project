#include "BankDB.hpp"
#include "CDate.hpp"
#include <iostream>

CDate askDate(const std::string& prompt); 

int main()
{
    BankDB db;

    /*  загрузка исходного файла  */
    try {
        db.load("data/accounts.csv");          // путь можно поменять
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << "\nStarting with empty DB.\n";
    }

    /*  бесконечное меню  */
    while (true)
    {
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
        if (!(std::cin >> choice)) break;   // Ctrl+D / Ctrl+Z

        switch (choice)
        {
        case 1:
            db.show();
            break;

        case 2:
            db.add();
            break;

        case 3: {
            std::size_t idx;
            std::cout << "Record number (1-based): ";
            std::cin >> idx;
            db.remove(idx);
            break;
        }

        case 4:
            db.sortById();
            break;

        case 5: {
            std::string id;
            std::cout << "ID to find: ";
            std::cin >> id;
            db.findById(id);
            break;
        }

        case 6: {
            CDate from = askDate("From (YYYY-MM-DD): ");
            CDate to   = askDate("To   (YYYY-MM-DD): ");
            db.rangeByOpenDate(from, to);
            break;
        }

        case 7: {                                 // пролонгация
            CDate today{2025,6,7};                // фиксированная дата, как в плане
            db.taskProlongation(today);
            break;
        }
        
        case 8: {                                 // прогноз
            int m;
            std::cout << "Months ahead (m): ";
            std::cin  >> m;
            std::ostringstream name;              // future_12.csv
            name << "future_" << m << ".csv";
            db.taskForecast(m, name.str());
            break;
        }

        case 9:
            db.save(); 
            break;

        case 10: {                              // ← открылся новый блок
            std::string out;
            std::cout << "File name: ";
            std::cin  >> out;
            db.save(out);
            break;
        }                                      // ← блок закрыт
            
        case 0:
            std::cout << "Bye!\n";
            return 0;

        default:
            std::cout << "Wrong choice.\n";
        }
    }
}
