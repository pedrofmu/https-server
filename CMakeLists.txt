cmake_minimum_required(VERSION 3.12)
project(httpProxy VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Change path from /src if needed, or add more directories
file(GLOB_RECURSE sources
        "${CMAKE_SOURCE_DIR}/src/*.c"
        )

# Añade el archivo fuente principal
add_executable(httpProxy ${sources})

# Añade el directorio de inclusión "include" al proyecto
target_include_directories(httpProxy PUBLIC ${CMAKE_SOURCE_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")
