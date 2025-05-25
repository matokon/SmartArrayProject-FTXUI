#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>

#include <string>
#include <vector>
#include <sstream>

#include "Employee.h"
#include "SmartArray.h"

using namespace ftxui;

SmartArray<Employee> employees;
std::string info_message;

void RunMenu();
void AddEmployee(ftxui::ScreenInteractive& screen);
void DisplayAll(ftxui::ScreenInteractive& screen);
void RemoveEmployee(ftxui::ScreenInteractive& screen);
void EditEmployee(ftxui::ScreenInteractive& screen);
void ClearAllEntries();
void SaveToFile();
void LoadFromFile();

int main() {
    RunMenu();
    return 0;
}

void RunMenu() {
    std::vector<std::string> entries = {
      "Dodaj pracownika",
      "Wyświetl wszystkich",
      "Usuń pracownika",
      "Edytuj pracownika",
      "Wyczyść wszystkich",
      "Zapisz do pliku",
      "Wczytaj z pliku",
      "Wyjście"
    };
    int selected = 0;
    auto menu = Menu(&entries, &selected, MenuOption::Vertical());
    auto screen = ScreenInteractive::TerminalOutput();

    menu |= CatchEvent([&](Event event) {
        if (event == Event::ArrowDown) {
            selected = (selected + 1) % entries.size();
            return true;
        }
        if (event == Event::ArrowUp) {
            selected = (selected + entries.size() - 1) % entries.size();
            return true;
        }
        if (event == Event::Return) {
            switch (selected) {
            case 0: AddEmployee(screen);    break;
            case 1: DisplayAll(screen);     break;
            case 2: RemoveEmployee(screen); break;
            case 3: EditEmployee(screen);   break;
            case 4: ClearAllEntries();      break;
            case 5: SaveToFile();           break;
            case 6: LoadFromFile();         break;
            case 7:
                screen.ExitLoopClosure()();
                return true;
            }
            return true;
        }
        return false;
        });

    auto renderer = Renderer(menu, [&] {
        return vbox({
          text("      Employee Manager      "),
          separator(),
          menu->Render(),
          separator(),
          text(info_message)
            });
        });

    screen.Loop(renderer);
}

void AddEmployee(ScreenInteractive& screen) {
    std::string name, surname, position, age_str, salary_str;
    info_message.clear();

    auto name_in = Input(&name, "Imię");
    auto surname_in = Input(&surname, "Nazwisko");
    auto age_in = Input(&age_str, "Wiek (int)");
    auto position_in = Input(&position, "Stanowisko");
    auto salary_in = Input(&salary_str, "Pensja (double)");
    auto add_btn = Button("Dodaj", [&] {
        try {
            int    age = std::stoi(age_str);
            double salary = std::stod(salary_str);
            employees.pushBack(Employee(name, surname, age, position, salary));
            info_message = "Dodano pracownika.";
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        screen.ExitLoopClosure()();
        });

    auto container = Container::Vertical({
      name_in, surname_in, age_in, position_in, salary_in, add_btn
        });

    auto renderer = Renderer(container, [&] {
        std::vector<Element> lines;
        lines.push_back(text(">> Dodaj pracownika"));
        lines.push_back(container->Render());
        lines.push_back(separator());
        lines.push_back(text(info_message));
        return vbox(lines);
        });

    screen.Loop(renderer);
}

void DisplayAll(ScreenInteractive& screen) {
    info_message.clear();

    auto renderer = Renderer([&] {
        std::vector<Element> lines;
        for (size_t i = 0; i < employees.size(); ++i) {
            std::ostringstream os;
            os << i << ": " << employees[i].ToString();
            lines.push_back(text(os.str()));
        }
        if (lines.empty())
            lines.push_back(text("Brak pracowników."));
        lines.push_back(separator());
        lines.push_back(text("Naciśnij Enter, aby wrócić..."));
        return vbox(lines) | border;
        });

    auto catch_enter = CatchEvent([&](Event e) {
        if (e == Event::Return) {
            screen.ExitLoopClosure()();
            return true;
        }
        return false;
        });

    screen.Loop(renderer | catch_enter);
}

void RemoveEmployee(ScreenInteractive& screen) {
    std::string idx_str;
    info_message.clear();

    auto idx_in = Input(&idx_str, "Indeks");
    auto del_btn = Button("Usuń", [&] {
        try {
            size_t idx = std::stoul(idx_str);
            if (idx < employees.size()) {
                employees.erase(idx);
                info_message = "Usunięto pracownika.";
            }
            else {
                info_message = "Nieprawidłowy indeks.";
            }
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        screen.ExitLoopClosure()();
        });

    auto container = Container::Vertical({ idx_in, del_btn });
    auto renderer = Renderer(container, [&] {
        std::vector<Element> lines;
        lines.push_back(text(">> Usuń pracownika"));
        lines.push_back(container->Render());
        lines.push_back(separator());
        lines.push_back(text(info_message));
        return vbox(lines) | border;
        });

    screen.Loop(renderer);
}

void EditEmployee(ScreenInteractive& screen) {
    std::string idx_str, name, surname, position, age_str, salary_str;
    info_message.clear();

    auto idx_in = Input(&idx_str, "Indeks");
    auto load_btn = Button("Wczytaj", [&] {
        try {
            size_t idx = std::stoul(idx_str);
            if (idx < employees.size()) {
                const auto& e = employees[idx];
                name = e.GetName();
                surname = e.GetSurname();
                age_str = std::to_string(e.GetAge());
                position = e.GetPosition();
                salary_str = std::to_string(e.GetSalary());
                info_message = "Dane załadowane.";
            }
            else {
                info_message = "Nieprawidłowy indeks.";
            }
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        });

    auto name_in = Input(&name, "Imię");
    auto surname_in = Input(&surname, "Nazwisko");
    auto age_in = Input(&age_str, "Wiek");
    auto position_in = Input(&position, "Stanowisko");
    auto salary_in = Input(&salary_str, "Pensja");
    auto save_btn = Button("Zapisz", [&] {
        try {
            size_t idx = std::stoul(idx_str);
            if (idx < employees.size()) {
                employees[idx].SetName(name);
                employees[idx].SetSurname(surname);
                employees[idx].SetAge(std::stoi(age_str));
                employees[idx].SetPosition(position);
                employees[idx].SetSalary(std::stod(salary_str));
                info_message = "Zmieniono dane.";
            }
            else {
                info_message = "Nieprawidłowy indeks.";
            }
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        screen.ExitLoopClosure()();
        });

    auto container = Container::Vertical({
      idx_in, load_btn,
      name_in, surname_in, age_in, position_in, salary_in,
      save_btn
        });

    auto renderer = Renderer(container, [&] {
        std::vector<Element> lines;
        lines.push_back(text(">> Edytuj pracownika"));
        lines.push_back(container->Render());
        lines.push_back(separator());
        lines.push_back(text(info_message));
        return vbox(lines) | border;
        });

    screen.Loop(renderer);
}

void ClearAllEntries() {
    employees.clear();
    info_message = "Usunięto wszystkich pracowników.";
}

void SaveToFile() {
    const std::string filename = "employees.txt";
    info_message = employees.SaveToFile(filename)
        ? "Zapisano do pliku: " + filename
        : "Błąd zapisu do pliku.";
}

void LoadFromFile() {
    const std::string filename = "employees.txt";
    info_message = employees.LoadFromFile(filename)
        ? "Wczytano z pliku: " + filename
        : "Błąd wczytywania z pliku.";
}