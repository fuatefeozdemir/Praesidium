#include "../../../include/Interface/World/MapRenderer.h"

#include "raylib.h"

#include "../../../include/Systems/ChunkSystem.h"

namespace {

    constexpr float TILE_SIZE_PIXELS = 32.0f;

    void DrawFloor(
        const Data::WorldData::Tile& tile,
        float posX,
        float posY) {

        Color color = DARKGRAY;

        switch (tile.floor) {

            case Data::WorldData::FloorType::DIRT:
                color = BROWN;
                break;

            case Data::WorldData::FloorType::SAND:
                color = YELLOW;
                break;

            case Data::WorldData::FloorType::WATER:
                color = BLUE;
                break;

            default:
                break;
        }

        DrawRectangleRec(
            { posX, posY, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS },
            color
        );

        // TODO: Move grid rendering to the debug renderer.
        DrawRectangleLinesEx(
            { posX, posY, TILE_SIZE_PIXELS, TILE_SIZE_PIXELS },
            1.0f,
            Fade(BLACK, 0.1f)
        );
    }

    void DrawOre(
        const Data::WorldData::Tile& tile,
        float posX,
        float posY) {

        if (tile.ore == Data::WorldData::ItemType::NONE) {
            return;
        }

        Color color = MAGENTA;

        switch (tile.ore) {

            case Data::WorldData::ItemType::IRON_ORE:
                color = ORANGE;
                break;

            case Data::WorldData::ItemType::COAL:
                color = BLACK;
                break;

            default:
                break;
        }

        const float padding = TILE_SIZE_PIXELS / 4.0f;
        const float size = TILE_SIZE_PIXELS / 2.0f;

        DrawRectangleRec(
            {
                posX + padding,
                posY + padding,
                size,
                size
            },
            color
        );
    }

    void DrawBuilding(
        const Data::WorldData::Building& building,
        float posX,
        float posY,
        float width,
        float height) {

        Color color = RED;

        switch (building.type) {

            case Data::WorldData::BuildingType::MINER:
                color = BLUE;
                break;

            case Data::WorldData::BuildingType::FURNACE:
                color = MAROON;
                break;

            case Data::WorldData::BuildingType::CONVEYOR_BELT:
                color = GREEN;
                break;

            default:
                break;
        }

        DrawRectangleRec(
            { posX, posY, width, height },
            color
        );

        DrawRectangleLinesEx(
            { posX, posY, width, height },
            2.0f,
            RAYWHITE
        );
    }

    void DrawBuildings(const Data::WorldData::Map& map) {

        // TODO:
        // Replace this with the future BuildingRenderer.
        // Buildings should eventually be rendered through
        // the spatial index instead of iterating over every building.

        for (int i = 0; i < map.buildingCount; ++i) {

            const auto& building = map.buildings[i];

            DrawBuilding(
                building,
                building.position.x * TILE_SIZE_PIXELS,
                building.position.y * TILE_SIZE_PIXELS,
                building.size.x * TILE_SIZE_PIXELS,
                building.size.y * TILE_SIZE_PIXELS
            );
        }
    }

    void DrawChunk(const Data::WorldData::Chunk& chunk) {

        for (int i = 0; i < Data::WorldData::CHUNK_SIZE * Data::WorldData::CHUNK_SIZE; ++i) {

            const auto& tile = chunk.tiles[i];

            const int localX = i % Data::WorldData::CHUNK_SIZE;
            const int localY = i / Data::WorldData::CHUNK_SIZE;

            const int worldX =
                chunk.position.x * Data::WorldData::CHUNK_SIZE + localX;

            const int worldY =
                chunk.position.y * Data::WorldData::CHUNK_SIZE + localY;

            const float posX = worldX * TILE_SIZE_PIXELS;
            const float posY = worldY * TILE_SIZE_PIXELS;

            DrawFloor(tile, posX, posY);
            DrawOre(tile, posX, posY);
        }
    }

    void DrawDebug(const Data::WorldData::Map&) {

        // TODO:
        // Render chunk boundaries, power networks,
        // logistics overlays and other debug information.
    }

} // namespace

namespace Interface::World::MapRenderer {

    void Draw(const Data::WorldData::Map& map) {

        // Draw only chunks inside the current camera view.
        for (const auto* chunk : Systems::ChunkSystem::GetVisibleChunks()) {
            DrawChunk(*chunk);
        }

        DrawBuildings(map);
        DrawDebug(map);
    }

} // namespace Interface::World::MapRenderer