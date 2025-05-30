#pragma once
#include <string>

class Employee {
public:
    Employee() = default;
    Employee(const std::string& name, const std::string& surname, int age, const std::string& position, double salary);

    const std::string& GetName() const { return m_Name; }
    const std::string& GetSurname() const { return m_Surname; }
    int GetAge() const { return m_Age; }
    const std::string& GetPosition() const { return m_Position; }
    double GetSalary() const { return m_Salary; }

    void SetName(const std::string& name) { m_Name = name; }
    void SetSurname(const std::string& surname) { m_Surname = surname; }
    void SetAge(int age) { m_Age = age; }
    void SetPosition(const std::string& position) { m_Position = position; }
    void SetSalary(double salary) { m_Salary = salary; }

    void Print() const;
    std::string ToString() const;

    std::string ToCSV() const;
    static Employee FromCSV(const std::string& line);


private:
    std::string m_Name;
    std::string m_Surname;
    int m_Age;
    std::string m_Position;
    double m_Salary;
};