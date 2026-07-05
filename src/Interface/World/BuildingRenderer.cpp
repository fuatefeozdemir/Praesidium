#include "../../../include/Interface/World/BuildingRenderer.h"
#include "raylib.h"

namespace Interface::World::BuildingRenderer {

    void Draw(const Data::CoreData::GameContext* context) {
        float tileSizeF = static_cast<float>(context->worldMap.tileSize);

        for (const auto& building : context->worldMap.activeBuildings) {

            // Veri katmanındaki int değerleri çizim anında float'a çevriliyor
            float posX = static_cast<float>(building.gridX) * tileSizeF;
            float posY = static_cast<float>(building.gridY) * tileSizeF;
            float renderWidth = static_cast<float>(building.width) * tileSizeF;
            float renderHeight = static_cast<float>(building.height) * tileSizeF;

            if (building.type == Data::WorldData::BuildingType::CORE_BASE) {
                Rectangle baseRec = { posX, posY, renderWidth, renderHeight };
                DrawRectangleRec(baseRec, Fade(SKYBLUE, 0.8f));

                Rectangle innerRec = { posX + tileSizeF, posY + tileSizeF, tileSizeF, tileSizeF };
                DrawRectangleRec(innerRec, DARKBLUE);

                DrawRectangleLinesEx(baseRec, 1.0f, BLUE);
            }
        }
    }
}