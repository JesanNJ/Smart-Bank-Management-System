cmake_minimum_required(VERSION 3.10)

project(AI_Driven_Bank_Management_System LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

add_executable(bank_management ${SOURCES})

target_include_directories(bank_management PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

