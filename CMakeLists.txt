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

# src ve include klasörlerinin içindeki tüm dosyaları otomatik bulur
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

add_executable(Praesidium ${SOURCES} ${HEADERS}
        include/Data/WorldData/ItemData.h
        include/Data/WorldData/RecipeData.h
        include/Data/WorldData/BuildingData.h
        include/Systems/World/BuildingSystem.h
        src/Systems/World/BuildingSystem.cpp
        include/Interface/World/BuildingRenderer.h
        src/Interface/World/BuildingRenderer.cpp
        include/Data/EntityData/ActionData.h
        include/Systems/Simulation/InteractionSystem.h
        src/Systems/Simulation/InteractionSystem.cpp
        include/Interface/World/FloatingText.h
        include/Systems/World/MapSystem.h
        src/Systems/World/MapSystem.cpp
        include/Data/UIData/BuildingMenuData.h
        include/Systems/UI/BuildingMenuSystem.h
        src/Systems/UI/BuildingMenuSystem.cpp
        include/Interface/UI/CoreUI/BuildingMenuRenderer.h
        src/Interface/UI/CoreUI/BuildingMenuRenderer.cpp
        include/Data/CoreData/GameContext.h
        src/Engine/Core/Application.cpp
        include/Engine/Core/GameStates.h
        src/Engine/Core/GameStates.cpp
)

target_link_libraries(Praesidium PRIVATE raylib)

target_compile_definitions(Praesidium PRIVATE ASSETS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/assets/\")