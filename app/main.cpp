#include <iostream>
#include <limits>
#include "SmartArray.h"
#include "Employee.h"

void PrintMenu() {
    std::cout << "\n===== MENU =====\n";
    std::cout << "1. Dodaj pracownika\n";
    std::cout << "2. Wyswietl wszystkich pracownikow\n";
    std::cout << "3. Edytuj pracownika\n";
    std::cout << "4. Usun pracownika\n";
    std::cout << "5. Liczba pracownikow\n";
    std::cout << "6. Wyczysc wszystkich\n";
    std::cout << "0. Wyjscie\n";
    std::cout << "================\n";
}

void ClearCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

Employee GetEmployeeFromInput() {
    std::string name, surname, position;
    int age;
    double salary;

    std::cout << "Imie: ";
    std::getline(std::cin, name);
    std::cout << "Nazwisko: ";
    std::getline(std::cin, surname);
    std::cout << "Wiek: ";
    std::cin >> age; ClearCin();
    std::cout << "Stanowisko: ";
    std::getline(std::cin, position);
    std::cout << "Pensja: ";
    std::cin >> salary; ClearCin();

    return Employee(name, surname, age, position, salary);
}

int main() {
    SmartArray<Employee> employees;
    int option;

    do {
        PrintMenu();
        std::cout << "Wybor: ";
        std::cin >> option;
        ClearCin();

        switch (option) {
        case 1: {
            std::cout << "\n--- Dodaj pracownika ---\n";
            employees.PushBack(GetEmployeeFromInput());
            break;
        }
        case 2: {
            std::cout << "\n--- Lista pracownikow ---\n";
            for (size_t i = 0; i < employees.Size(); ++i) {
                std::cout << i << ": ";
                employees[i].Print();
            }
            break;
        }
        case 3: {
            size_t index;
            std::cout << "Podaj indeks do edycji: ";
            std::cin >> index; ClearCin();
            if (index >= employees.Size()) {
                std::cout << "Nieprawidlowy indeks!\n";
                break;
            }
            std::cout << "Podaj nowe dane:\n";
            employees.EditElem(index, GetEmployeeFromInput());
            break;
        }
        case 4: {
            size_t index;
            std::cout << "Podaj indeks do usuniecia: ";
            std::cin >> index; ClearCin();
            if (index >= employees.Size()) {
                std::cout << "Nieprawidlowy indeks!\n";
                break;
            }
            employees.RemoveElem(index);
            break;
        }
        case 5:
            std::cout << "Liczba pracownikow: " << employees.Size() << "\n";
            break;
        case 6:
            employees.Clear();
            std::cout << "Wszyscy pracownicy usunieci.\n";
            break;
        case 0:
            std::cout << "Koniec programu.\n";
            break;
        default:
            std::cout << "Nieprawidlowy wybor!\n";
        }
    } while (option != 0);

    return 0;
}
