#include "../../../include/Interface/World/MapRenderer.h"
#include "raylib.h"

namespace Interface::World::MapRenderer {

    void Draw(const Data::WorldData::Map& map) {
        for (int y = 0; y < map.height; y++) {
            for (int x = 0; x < map.width; x++) {
                int index = y * map.width + x;
                const auto& tile = map.tiles[index];

                int posX = x * map.tileSize;
                int posY = y * map.tileSize;

                // Zemin Rengi
                Color tileColor = DARKGRAY;
                if (tile.floor == Data::WorldData::FloorType::DIRT) tileColor = BROWN;
                else if (tile.floor == Data::WorldData::FloorType::SAND) tileColor = YELLOW;
                else if (tile.floor == Data::WorldData::FloorType::WATER) tileColor = BLUE;

                DrawRectangle(posX, posY, map.tileSize, map.tileSize, tileColor);
                DrawRectangleLines(posX, posY, map.tileSize, map.tileSize, Fade(BLACK, 0.3f));

                // Maden Rengi
                if (tile.ore != Data::WorldData::OreType::NONE) {
                    Color oreColor = WHITE;
                    if (tile.ore == Data::WorldData::OreType::IRON) oreColor = LIGHTGRAY;
                    else if (tile.ore == Data::WorldData::OreType::COPPER) oreColor = ORANGE;
                    else if (tile.ore == Data::WorldData::OreType::COAL) oreColor = BLACK;

                    int padding = map.tileSize / 4;
                    DrawRectangle(posX + padding, posY + padding, map.tileSize / 2, map.tileSize / 2, oreColor);
                }
            }
        }
    }
}