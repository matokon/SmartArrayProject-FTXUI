cmake_minimum_required(VERSION 3.10)
project(SmartArrayProject LANGUAGES CXX)

# Standard C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Biblioteka header-only (SmartArray.h)
add_library(SmartArray INTERFACE)
target_include_directories(SmartArray INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Aplikacja g³ówna – dodaj Ÿród³a C++ (main.cpp i Employee.cpp)
add_executable(MainApp
    app/main.cpp
    src/Employee.cpp      # <-- dodajemy Ÿród³o implementacji
)

# Ustaw katalog include/ jako katalog nag³ówków dla MainApp
target_include_directories(MainApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Linkowanie biblioteki SmartArray
target_link_libraries(MainApp SmartArray)
