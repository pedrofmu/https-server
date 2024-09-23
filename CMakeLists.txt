cmake_minimum_required(VERSION 3.12)

# Definir el proyecto y su versión
project(https-server VERSION 1.0)

# Exportar los comandos de compilación (útil para herramientas como clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Buscar el paquete OpenSSL (obligatorio para compilar)
find_package(OpenSSL REQUIRED)

# Recopilar todos los archivos fuente de forma recursiva
file(GLOB_RECURSE sources
        "${CMAKE_SOURCE_DIR}/src/*.c"
        )

# Añadir el ejecutable principal
add_executable(https-server ${sources})

# Enlazar las librerías de OpenSSL al ejecutable https-server
target_link_libraries(https-server OpenSSL::SSL OpenSSL::Crypto)

# Añadir el directorio "include" para los archivos de cabecera (si es aplicable)
target_include_directories(https-server PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Añadir símbolos de depuración
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g")

