cmake_minimum_required(VERSION 3.10)
project(SmartArrayProject LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(
  ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(SmartArray INTERFACE)
target_include_directories(SmartArray INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(MainApp
    app/main.cpp
    src/Employee.cpp      
)

target_include_directories(MainApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(MainApp SmartArray ftxui::component)
