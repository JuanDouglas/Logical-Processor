cmake_minimum_required(VERSION 3.16)

project(Logical_Processor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Logical_Processor
        main.c

        refs/base/exp.c
        refs/base/circuit.c
        refs/base/clock.c

        refs/circuits.c
)

target_include_directories(Logical_Processor PRIVATE
        refs
        refs/base
)
