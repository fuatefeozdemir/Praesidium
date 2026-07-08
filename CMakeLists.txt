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
        src/Systems/BuildingSystem.cpp
        include/Interface/World/BuildingRenderer.h
        src/Interface/World/BuildingRenderer.cpp
        include/Data/EntityData/ActionData.h
        src/Systems/InteractionSystem.cpp
        include/Interface/World/FloatingText.h
        src/Systems/MapSystem.cpp
        include/Data/UIData/BuildingMenuData.h
        src/Systems/BuildingMenuSystem.cpp
        src/Interface/UI/BuildingMenuRenderer.cpp
        include/Data/CoreData/GameContext.h
        src/Engine/Core/Application.cpp
        include/Engine/Core/GameStates.h
        src/Engine/Core/GameStates.cpp
        include/Interface/UI/BuildingMenuRenderer.h
        include/Interface/UI/MainMenuRenderer.h
        include/Systems/PlayerSystem.h
        include/Systems/BuildingMenuSystem.h
        include/Systems/MapSystem.h
        include/Data/CoreData/Vector2Int.h
        include/Systems/CameraSystem.h
        src/Systems/CameraSystem.cpp
)

target_link_libraries(Praesidium PRIVATE raylib)

target_compile_definitions(Praesidium PRIVATE ASSETS_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/assets/\")