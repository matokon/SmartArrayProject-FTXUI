#include "Employee.h"
#include <iostream>
#include <sstream>

Employee::Employee(const std::string& name, const std::string& surname, int age, const std::string& position, double salary)
    : m_Name(name), m_Surname(surname), m_Age(age), m_Position(position), m_Salary(salary) {
}

void Employee::Print() const {
    std::cout << m_Name << " " << m_Surname << ", wiek: " << m_Age
        << ", stanowisko: " << m_Position << ", pensja: " << m_Salary << "\n";
}

std::string Employee::ToString() const {
    std::ostringstream os;
    os << m_Name << " " << m_Surname << ", wiek: " << m_Age
       << ", stanowisko: " << m_Position << ", pensja: " << m_Salary;
    return os.str();
}

std::string Employee::ToCSV() const {
    return m_Name + "," + m_Surname + "," + std::to_string(m_Age) + "," + m_Position + "," + std::to_string(m_Salary);
}

Employee Employee::FromCSV(const std::string& line) {
    std::istringstream iss(line);
    std::string name, surname, position;
    int age;
    double salary;

    std::getline(iss, name, ',');
    std::getline(iss, surname, ',');
    
    std::string ageStr;
    std::getline(iss, ageStr, ',');
    age = std::stoi(ageStr);

    std::getline(iss, position, ',');

    std::string salaryStr;
    std::getline(iss, salaryStr, ',');
    salary = std::stod(salaryStr);

    return Employee(name, surname, age, position, salary);
}