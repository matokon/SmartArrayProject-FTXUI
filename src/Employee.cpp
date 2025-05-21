#include "Employee.h"
#include <iostream>

Employee::Employee(const std::string& name, const std::string& surname, int age, const std::string& position, double salary)
    : m_Name(name), m_Surname(surname), m_Age(age), m_Position(position), m_Salary(salary) {
}

void Employee::Print() const {
    std::cout << m_Name << " " << m_Surname << ", wiek: " << m_Age
        << ", stanowisko: " << m_Position << ", pensja: " << m_Salary << "\n";
}
