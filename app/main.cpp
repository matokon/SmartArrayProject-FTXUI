#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include "SmartArray.h"
#include "Employee.h"
#include <sstream>

using namespace ftxui;

int main() {
    ScreenInteractive screen = ScreenInteractive::TerminalOutput();

    SmartArray<Employee> employees;

    // ----------- MENU ------------
    std::vector<std::string> menu_entries = {
        "Dodaj pracownika",
        "Wyswietl wszystkich",
        "Usun pracownika",
        "Wyczysc wszystkich",
        "Wyjscie"
    };
    int selected = 0;
    auto menu = Menu(&menu_entries, &selected);

    // ----------- ADD EMPLOYEE ------------
    std::string name, surname, position, age_str, salary_str;
    std::string info_message;

    auto name_in     = Input(&name,     "Imie");
    auto surname_in  = Input(&surname,  "Nazwisko");
    auto age_in      = Input(&age_str,  "Wiek (int)");
    auto position_in = Input(&position, "Stanowisko");
    auto salary_in   = Input(&salary_str, "Pensja (double)");
    auto add_btn     = Button("Dodaj", [&] {
        try {
            int    age    = std::stoi(age_str);
            double salary = std::stod(salary_str);
            employees.PushBack(Employee(name, surname, age, position, salary));
            name.clear(); surname.clear(); position.clear(); age_str.clear(); salary_str.clear();
            info_message = "Dodano pracownika.";
        } catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
    });
    auto add_container = Container::Vertical({ name_in, surname_in, age_in, position_in, salary_in, add_btn });

    // ----------- DELETE EMPLOYEE ------------
    std::string delete_index_str;
    auto delete_index_in = Input(&delete_index_str, "Indeks");
    auto delete_btn      = Button("Usuń", [&] {
        try {
            size_t idx = std::stoul(delete_index_str);
            if (idx < employees.Size()) {
                employees.RemoveElem(idx);
                info_message = "Usunięto pracownika.";
            } else {
                info_message = "Nieprawidłowy indeks.";
            }
        } catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
    });
    auto delete_container = Container::Vertical({ delete_index_in, delete_btn });

    // ----------- CLEAR ALL ------------
    auto clear_btn = Button("Usuń wszystkich", [&] {
        employees.Clear();
        info_message = "Usunięto wszystkich pracowników.";
    });
    auto clear_container = Container::Vertical({ clear_btn });

    // ----------- TAB CONTAINER ------------
    auto content_container = Container::Tab({
        add_container,          // 0 Dodaj
        Container::Vertical({}),// 1 Wyswietl wszystkich (placeholder; rendered dynamically)
        delete_container,       // 2 Usun
        clear_container,        // 3 Wyczyść
        Container::Vertical({}) // 4 Wyjście
    }, &selected);

    auto root = Container::Horizontal({ menu, content_container });

    // ----------- RENDERER ------------
    auto renderer = Renderer(root, [&] {
        Element sidebar = menu->Render() | frame;

        Element main_window;
        switch (selected) {
            case 0:
                main_window = add_container->Render();
                break;

            case 1: { // list all employees
                std::vector<Element> lines;
                for (size_t i = 0; i < employees.Size(); ++i) {
                    std::ostringstream os;
                    os << i << ": " << employees[i].ToString();
                    lines.push_back(text(os.str()));
                }
                if (lines.empty())
                    lines.push_back(text("Brak pracowników."));
                main_window = vbox(std::move(lines));
                break;
            }

            case 2:
                main_window = delete_container->Render();
                break;

            case 3:
                main_window = clear_container->Render();
                break;

            case 4:
                screen.Exit();
                main_window = text("Zamykam...");
                break;
        }

        if (!info_message.empty()) {
            main_window = vbox({ main_window, separator(), text(info_message) | color(Color::Green) });
        }

        return hbox({
            sidebar | flex_shrink,
            separator(),
            main_window | flex
        }) | flex;
    });

    // ----------- EVENT LOOP ------------
    screen.Loop(renderer);
    return 0;
}
