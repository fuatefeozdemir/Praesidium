#include "../../include/Systems/MapSystem.h"

namespace Systems::MapSystem {

    // ==========================================
    // KOORDİNAT VE YARDIMCI FONKSİYONLAR
    // ==========================================

    Data::CoreData::Vector2Int WorldToChunk(Data::CoreData::Vector2Int worldPos) {
        // Tamsayı bölmesinde negatif koordinatların 0'a yuvarlanmasını engelleyen formül
        int chunkX = worldPos.x >= 0 ? worldPos.x / Data::WorldData::CHUNK_SIZE : (worldPos.x - Data::WorldData::CHUNK_SIZE + 1) / Data::WorldData::CHUNK_SIZE;
        int chunkY = worldPos.y >= 0 ? worldPos.y / Data::WorldData::CHUNK_SIZE : (worldPos.y - Data::WorldData::CHUNK_SIZE + 1) / Data::WorldData::CHUNK_SIZE;
        return {chunkX, chunkY};
    }

    Data::CoreData::Vector2Int WorldToLocal(Data::CoreData::Vector2Int worldPos) {
        Data::CoreData::Vector2Int chunkPos = WorldToChunk(worldPos);
        int localX = worldPos.x - (chunkPos.x * Data::WorldData::CHUNK_SIZE);
        int localY = worldPos.y - (chunkPos.y * Data::WorldData::CHUNK_SIZE);
        return {localX, localY};
    }

    int LocalToIndex(Data::CoreData::Vector2Int localPos) {
        return localPos.y * Data::WorldData::CHUNK_SIZE + localPos.x;
    }

    bool IsInsideWorld(const Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos) {
        // 0 değeri sınır olmadığını (sonsuz harita) ifade eder
        if (map.worldWidthChunks > 0 && map.worldHeightChunks > 0) {
            if (chunkPos.x < 0 || chunkPos.x >= map.worldWidthChunks || chunkPos.y < 0 || chunkPos.y >= map.worldHeightChunks) {
                return false;
            }
        }
        return true;
    }

    // ==========================================
    // TILE ERİŞİM API
    // ==========================================

    void SetTileBuildingID(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos, Data::WorldData::BuildingId id) {
        Data::WorldData::Tile* tile = GetOrCreateTile(map, worldPos);
        if (tile) {
            tile->buildingID = id;
        }
    }

    Data::WorldData::Tile* GetTile(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos) {
        Data::WorldData::Chunk* chunk = GetChunkFromWorldPos(map, worldPos);
        if (!chunk) return nullptr;

        return &chunk->tiles[LocalToIndex(WorldToLocal(worldPos))];
    }

    Data::WorldData::Tile* GetOrCreateTile(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos) {
        Data::CoreData::Vector2Int chunkPos = WorldToChunk(worldPos);

        if (!IsInsideWorld(map, chunkPos)) {
            return nullptr;
        }

        Data::WorldData::Chunk& chunk = GetOrCreateChunk(map, chunkPos);
        return &chunk.tiles[LocalToIndex(WorldToLocal(worldPos))];
    }

    // ==========================================
    // CHUNK YÖNETİM API
    // ==========================================

    void Initialize(Data::WorldData::Map& map, uint64_t seed, int widthChunks, int heightChunks) {
        map.chunks.clear();
        map.worldSeed = seed;
        map.worldWidthChunks = widthChunks;
        map.worldHeightChunks = heightChunks;
        map.nextBuildingID = 1;

        // Havuz sayaçlarının sıfırlanması yeni entity tahsislerini başa sarar
        map.activeBuildingCount = 0;
        map.activeHealthCount = 0;
        map.activeInventoryCount = 0;
        map.activeProductionCount = 0;
        map.activeExtractorCount = 0;
        map.activePowerProducerCount = 0;
        map.activePowerConsumerCount = 0;
        map.activeConveyorCount = 0;
        map.activeSplitterCount = 0;
    }

    Data::WorldData::Chunk* GetChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos) {
        auto it = map.chunks.find(chunkPos);
        if (it != map.chunks.end()) {
            return &it->second;
        }
        return nullptr;
    }

    Data::WorldData::Chunk* GetChunkFromWorldPos(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos) {
        return GetChunk(map, WorldToChunk(worldPos));
    }

    Data::WorldData::Chunk& CreateChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos) {
        // Mevcut bir Chunk varsa veri kaybını (Data Wipe) önlemek için işlemi iptal et
        auto it = map.chunks.find(chunkPos);
        if (it != map.chunks.end()) {
            return it->second;
        }

        auto& chunk = map.chunks[chunkPos];

        // Struct içindeki potansiyel çöp verileri (Garbage Data) temizle
        chunk = {};

        chunk.position = chunkPos;
        chunk.isLoaded = true;
        chunk.needsSimulation = true;
        chunk.isModified = false;

        // Zemin ve varsayılan Tile değerlerinin atanması
        for (int i = 0; i < Data::WorldData::CHUNK_SIZE * Data::WorldData::CHUNK_SIZE; ++i) {
            chunk.tiles[i].floor = Data::WorldData::FloorType::STONE;
            chunk.tiles[i].ore = Data::WorldData::ItemType::NONE;
            chunk.tiles[i].buildingID = -1;
            chunk.tiles[i].isPassable = true;
        }

        return chunk;
    }

    Data::WorldData::Chunk& GetOrCreateChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos) {
        auto it = map.chunks.find(chunkPos);
        if (it != map.chunks.end()) {
            return it->second;
        }
        return CreateChunk(map, chunkPos);
    }
}