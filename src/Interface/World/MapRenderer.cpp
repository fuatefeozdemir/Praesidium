#include "../../../include/Interface/World/MapRenderer.h"
#include "raylib.h"

namespace Interface::World::MapRenderer {

    void Draw(const Data::CoreData::GameContext* context) {
        const auto& map = context->worldMap;

        for (int y = 0; y < map.height; y++) {
            for (int x = 0; x < map.width; x++) {
                int index = y * map.width + x;
                const auto& tile = map.tiles[index];

                // Veri katmanındaki int değerleri sadece çizim anında float'a çevriliyor
                float posX = static_cast<float>(x * map.tileSize);
                float posY = static_cast<float>(y * map.tileSize);
                float tileSizeF = static_cast<float>(map.tileSize);

                // Zemin Rengi
                Color tileColor = DARKGRAY;
                if (tile.floor == Data::WorldData::FloorType::DIRT) tileColor = BROWN;
                else if (tile.floor == Data::WorldData::FloorType::SAND) tileColor = YELLOW;
                else if (tile.floor == Data::WorldData::FloorType::WATER) tileColor = BLUE;

                Rectangle tileRec = { posX, posY, tileSizeF, tileSizeF };

                DrawRectangleRec(tileRec, tileColor);
                DrawRectangleLinesEx(tileRec, 1.0f, Fade(BLACK, 0.3f));

                // Maden Rengi
                if (tile.ore != Data::WorldData::OreType::NONE) {
                    Color oreColor = WHITE;
                    if (tile.ore == Data::WorldData::OreType::IRON) oreColor = LIGHTGRAY;
                    else if (tile.ore == Data::WorldData::OreType::COPPER) oreColor = ORANGE;
                    else if (tile.ore == Data::WorldData::OreType::COAL) oreColor = BLACK;

                    float padding = tileSizeF / 4.0f;
                    Rectangle oreRec = { posX + padding, posY + padding, tileSizeF / 2.0f, tileSizeF / 2.0f };
                    DrawRectangleRec(oreRec, oreColor);
                }
            }
        }
    }
}