#pragma once
#include <vector>
#include <cstdint>

namespace Data::WorldData {

    enum class FloorType : uint8_t {
        STONE, // İnşaat yapılabilir, standart hız
        DIRT,  // İnşaat yapılabilir
        SAND,  // İnşaat yapılabilir, yavaş yürünür
        WATER  // Üzerine sadece köprü/pompa kurulabilir, yürünemez
    };

    enum class OreType : uint8_t {
        NONE,  // Maden yok
        IRON,  // Demir
        COPPER,// Bakır
        COAL   // Kömür
    };

    struct Tile {
        FloorType floor;
        OreType ore;

        int buildingID = -1;
        bool isPassable;
    };

    struct Map {
        int width;
        int height;
        int tileSize;
        std::vector<Tile> tiles;
    };
}