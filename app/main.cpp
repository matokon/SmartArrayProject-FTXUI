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
        "Dodaj pracownika",     // 0
        "Wyswietl wszystkich",  // 1
        "Usun pracownika",      // 2
        "Edytuj pracownika",    // 3
        "Wyczysc wszystkich",   // 4
        "Zapisz / Wczytaj",     // 5
        "Wyjscie"
    };
    int selected = 0;
    auto menu = Menu(&menu_entries, &selected);

    // ----------- ADD EMPLOYEE ------------
    std::string name, surname, position, age_str, salary_str;
    std::string info_message;

    std::string filename = "employees.txt";
    auto filename_input = Input(&filename, "Nazwa pliku");

    auto save_to_file_btn = Button("Zapisz", [&] {
        if (employees.SaveToFile(filename)) {
            info_message = "Zapisano do pliku: " + filename;
        }
        else {
            info_message = "Nie udało się zapisać.";
        }
        });

    auto load_from_file_btn = Button("Wczytaj", [&] {
        if (employees.LoadFromFile(filename)) {
            info_message = "Wczytano z pliku: " + filename;
        }
        else {
            info_message = "Nie udało się wczytać.";
        }
        });

    auto save_load_container = Container::Vertical({
        filename_input,
        save_to_file_btn,
        load_from_file_btn
        });


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

    // ----------- EDIT EMPLOYEE ------------
    std::string edit_index_str;
    std::string edit_name, edit_surname, edit_position, edit_age_str, edit_salary_str;

    auto edit_index_input = Input(&edit_index_str, "Indeks");

    auto load_btn = Button("Wczytaj dane", [&] {
        try {
            size_t idx = std::stoul(edit_index_str);
            if (idx < employees.Size()) {
                const auto& emp = employees[idx];
                edit_name = emp.GetName();
                edit_surname = emp.GetSurname();
                edit_age_str = std::to_string(emp.GetAge());
                edit_position = emp.GetPosition();
                edit_salary_str = std::to_string(emp.GetSalary());
                info_message = "Wczytano dane pracownika.";
            }
            else {
                info_message = "Nieprawidłowy indeks.";
            }
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        });

    auto edit_name_in = Input(&edit_name, "Imie");
    auto edit_surname_in = Input(&edit_surname, "Nazwisko");
    auto edit_age_in = Input(&edit_age_str, "Wiek");
    auto edit_position_in = Input(&edit_position, "Stanowisko");
    auto edit_salary_in = Input(&edit_salary_str, "Pensja");

    auto save_btn = Button("Zapisz zmiany", [&] {
        try {
            size_t idx = std::stoul(edit_index_str);
            if (idx < employees.Size()) {
                employees[idx].SetName(edit_name);
                employees[idx].SetSurname(edit_surname);
                employees[idx].SetAge(std::stoi(edit_age_str));
                employees[idx].SetPosition(edit_position);
                employees[idx].SetSalary(std::stod(edit_salary_str));
                info_message = "Zmieniono dane pracownika.";
            }
            else {
                info_message = "Nieprawidłowy indeks.";
            }
        }
        catch (...) {
            info_message = "Błąd danych wejściowych.";
        }
        });

    auto edit_container = Container::Vertical({
        edit_index_input,
        load_btn,
        edit_name_in,
        edit_surname_in,
        edit_age_in,
        edit_position_in,
        edit_salary_in,
        save_btn
        });

    // ----------- TAB CONTAINER ------------
    auto content_container = Container::Tab({
        add_container,
        Container::Vertical({}),
        delete_container,
        edit_container,
        clear_container,
        save_load_container,
        Container::Vertical({})
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

            case 1: {
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
                main_window = edit_container->Render();
                break;

            case 4:
                main_window = clear_container->Render();
                break;

            case 5:
                main_window = save_load_container->Render();
                break;

            case 6:
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
