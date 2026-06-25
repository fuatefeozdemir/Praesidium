#include "../../../include/Interface/World/BuildingRenderer.h"
#include "raylib.h"

namespace Interface::World::BuildingRenderer {

    void Draw(const Data::CoreData::GameContext* context) {
        int tileSize = context->worldMap.tileSize;

        for (const auto& building : context->worldMap.activeBuildings) {
            
            int posX = building.gridX * tileSize;
            int posY = building.gridY * tileSize;
            int renderWidth = building.width * tileSize;
            int renderHeight = building.height * tileSize;

            if (building.type == Data::WorldData::BuildingType::CORE_BASE) {
                DrawRectangle(posX, posY, renderWidth, renderHeight, Fade(SKYBLUE, 0.8f));
                DrawRectangle(posX + tileSize, posY + tileSize, tileSize, tileSize, DARKBLUE);
                DrawRectangleLines(posX, posY, renderWidth, renderHeight, BLUE);
            }
        }
    }
}