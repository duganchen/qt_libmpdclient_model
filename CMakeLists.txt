cmake_minimum_required(VERSION 3.11)
project(quetzalcoatl LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 5.14 COMPONENTS Core Widgets Gui Test REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBMPDCLIENT REQUIRED IMPORTED_TARGET libmpdclient)

add_subdirectory(mpdclient)

add_executable(queueview queuemodel.cpp main.cpp mpdConnectionManager.cpp mainwindow.cpp)
target_include_directories(queueview PRIVATE ${PROJECT_SOURCE_DIR}/mpdclient/include)
target_link_libraries(queueview PRIVATE Qt5::Widgets Qt5::Core mpdclient PkgConfig::LIBMPDCLIENT)

enable_testing()
add_subdirectory(tests)