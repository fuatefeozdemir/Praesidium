#include "../../../include/Interface/World/MapRenderer.h"
#include "raylib.h"

namespace Interface::World::MapRenderer {

    // Şimdilik sabit piksel boyutu (İleride kamera zoom'u ile dinamikleşecek)
    constexpr float TILE_PIXEL_SIZE = 32.0f;

    // ==========================================
    // ALT ÇİZİM FONKSİYONLARI (İSKELET)
    // ==========================================

    void DrawFloor(const Data::WorldData::Tile& tile, float posX, float posY) {
        Color color = DARKGRAY; // Varsayılan STONE

        if (tile.floor == Data::WorldData::FloorType::DIRT) color = BROWN;
        else if (tile.floor == Data::WorldData::FloorType::SAND) color = YELLOW;
        else if (tile.floor == Data::WorldData::FloorType::WATER) color = BLUE;

        DrawRectangleRec({posX, posY, TILE_PIXEL_SIZE, TILE_PIXEL_SIZE}, color);

        // Debug Grid (Şimdilik açık kalması testler için iyi)
        DrawRectangleLinesEx({posX, posY, TILE_PIXEL_SIZE, TILE_PIXEL_SIZE}, 1.0f, Fade(BLACK, 0.1f));
    }

    void DrawOre(const Data::WorldData::Tile& tile, float posX, float posY) {
        if (tile.ore == Data::WorldData::ItemType::NONE) return;

        Color oreColor = MAGENTA; // Hata rengi (Bilinmeyen maden)
        if (tile.ore == Data::WorldData::ItemType::IRON_ORE) oreColor = ORANGE;
        else if (tile.ore == Data::WorldData::ItemType::COAL) oreColor = BLACK;
        // COPPER vb. eklendikçe burası genişleyecek

        float padding = TILE_PIXEL_SIZE / 4.0f;
        float size = TILE_PIXEL_SIZE / 2.0f;
        DrawRectangleRec({posX + padding, posY + padding, size, size}, oreColor);
    }

    void DrawBuilding(const Data::WorldData::Building& building, float posX, float posY, float widthX, float heightY) {
        Color buildColor = RED; // Varsayılan

        // İleride her bina kendi özel çizim fonksiyonuna (DrawMiner, DrawConveyor vb.) yönlendirilecek
        switch (building.type) {
            case Data::WorldData::BuildingType::MINER:
                buildColor = BLUE;
                break;
            case Data::WorldData::BuildingType::FURNACE:
                buildColor = MAROON;
                break;
            case Data::WorldData::BuildingType::CONVEYOR_BELT:
                buildColor = GREEN;
                break;
            default:
                break;
        }

        DrawRectangleRec({posX, posY, widthX, heightY}, buildColor);

        // Bina sınırlarını belli et
        DrawRectangleLinesEx({posX, posY, widthX, heightY}, 2.0f, RAYWHITE);
    }

    void DrawBuildings(const Data::WorldData::Map& map) {
        // DOD Optimizasyonu: Binaları Tile'lardan değil, doğrudan aktif havuzdan O(N) ile çiz
        for (int i = 0; i < map.activeBuildingCount; ++i) {
            const auto& building = map.buildings[i];

            float posX = building.position.x * TILE_PIXEL_SIZE;
            float posY = building.position.y * TILE_PIXEL_SIZE;
            float widthX = building.size.x * TILE_PIXEL_SIZE;
            float heightY = building.size.y * TILE_PIXEL_SIZE;

            DrawBuilding(building, posX, posY, widthX, heightY);
        }
    }

    void DrawChunk(const Data::WorldData::Chunk& chunk) {
        if (!chunk.isLoaded) return;

        for (int i = 0; i < Data::WorldData::CHUNK_SIZE * Data::WorldData::CHUNK_SIZE; ++i) {
            const auto& tile = chunk.tiles[i];

            // Chunk içindeki 1D index'i 2D local koordinata çevir
            int localX = i % Data::WorldData::CHUNK_SIZE;
            int localY = i / Data::WorldData::CHUNK_SIZE;

            // Dünya koordinatını hesapla (Piksel pozisyonu için)
            int worldX = (chunk.position.x * Data::WorldData::CHUNK_SIZE) + localX;
            int worldY = (chunk.position.y * Data::WorldData::CHUNK_SIZE) + localY;

            float posX = worldX * TILE_PIXEL_SIZE;
            float posY = worldY * TILE_PIXEL_SIZE;

            DrawFloor(tile, posX, posY);
            DrawOre(tile, posX, posY);
        }
    }

    void DrawDebug(const Data::WorldData::Map& map) {
        // İleride chunk sınırları, tile indeksleri, güç ağları (power network) ve lojistik oklar buraya eklenecek
    }

    // ==========================================
    // ANA ÇİZİM DÖNGÜSÜ
    // ==========================================

    void Draw(const Data::WorldData::Map& map) {
        // 1. Önce zemin ve madenleri (Statik dünya) çiz
        for (const auto& pair : map.chunks) {
            // İleride Culling (Görüş alanı kontrolü) buraya eklenecek
            // if (!IsChunkVisible(pair.second, camera)) continue;

            DrawChunk(pair.second);
        }

        // 2. Üzerine binaları çiz
        DrawBuildings(map);

        // 3. Debug verilerini çiz
        DrawDebug(map);
    }
}