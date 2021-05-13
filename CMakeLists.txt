cmake_minimum_required(VERSION 3.17)
project(OOP_2)

set(CMAKE_CXX_STANDARD 14)

add_executable(OOP_2 main.cpp line.cpp line.h getlines.cpp Menu.cpp timetable.cpp timetable.h tests/Station_test.h tests/Line_test.h tests/CrossingStation_test.h tests/Timetable_test.h test.cpp railway_system.cpp railway_system.h span.cpp extern.cpp)
