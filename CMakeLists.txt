cmake_minimum_required(VERSION 3.29)
project(Praesidium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/master.tar.gz
)
FetchContent_MakeAvailable(raylib)

# src ve include klasörlerinin içindeki (ve alt klasörlerindeki) tüm dosyaları otomatik bulur
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Bulunan tüm dosyaları derleyiciye tek seferde teslim eder
add_executable(Praesidium ${SOURCES} ${HEADERS}
        include/Interface/UIData.h
        include/Interface/MenuController.h
        src/Interface/MenuController.cpp
        include/Interface/RenderLayer.h
        src/Interface/RenderLayer.cpp
)

target_link_libraries(Praesidium PRIVATE raylib)