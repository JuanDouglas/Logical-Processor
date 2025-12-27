cmake_minimum_required(VERSION 3.16)

project(Logical_Processor C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(Logical_Processor
        main.c

        refs/base/exp.c
        refs/base/circuit.c
        refs/base/clock.c
        refs/base/unit.c

        refs/circuits.c
        refs/loader.c
)

target_include_directories(Logical_Processor PRIVATE
        refs
        refs/base
)

add_custom_command(TARGET Logical_Processor POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_SOURCE_DIR}/cpu.txt"
        "$<TARGET_FILE_DIR:Logical_Processor>/cpu.txt"
        COMMENT "Copiando cpu.txt para a pasta do executavel..."
)
