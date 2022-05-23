cmake_minimum_required(VERSION 3.20)
project(HW2RES)

set(CMAKE_CXX_STANDARD 11)

add_executable(HW2RES main.cpp Configuration.cpp User_Interaction.cpp User_Interaction.h Neverland.h Neverland.cpp Graph.h Validation.h Validation.cpp exceptions.cpp)
