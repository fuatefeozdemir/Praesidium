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
        include/Data/UIData/UIData.h
        include/Data/CoreData/AppState.h
        include/Interface/UI/CoreUI/MainMenuSystem.h
        src/Interface/UI/CoreUI/MainMenuSystem.cpp
        include/Engine/Core/CameraManager.h
        src/Engine/Core/CameraManager.cpp
        include/Data/EntityData/PlayerData.h
        include/Systems/Simulation/PlayerSystem.h
        src/Systems/Simulation/PlayerSystem.cpp
        include/Data/WorldData/MapData.h
        include/Systems/Simulation/MapSystem.h
        src/Systems/Simulation/MapSystem.cpp
        include/Interface/World/MapRenderer.h
        src/Interface/World/MapRenderer.cpp
)

target_link_libraries(Praesidium PRIVATE raylib)