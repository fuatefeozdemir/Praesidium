#include "../../include/Systems/MapSystem.h"

#include "../../include/Engine/Config/WorldConfig.h"

namespace {

    int FloorDivide(int value, int divisor) {
        return value >= 0
            ? value / divisor
            : (value - divisor + 1) / divisor;
    }

} // namespace

namespace Systems::MapSystem {

    Data::CoreData::Vector2Int PixelToTile(int pixelX, int pixelY) {
        return {
            FloorDivide(pixelX, Engine::Config::TILE_SIZE),
            FloorDivide(pixelY, Engine::Config::TILE_SIZE)
        };
    }

    Data::CoreData::Vector2Int WorldToChunk(Data::CoreData::Vector2Int worldPos) {
        return {
            FloorDivide(worldPos.x, Data::WorldData::CHUNK_SIZE),
            FloorDivide(worldPos.y, Data::WorldData::CHUNK_SIZE)
        };
    }

    Data::CoreData::Vector2Int WorldToLocal(Data::CoreData::Vector2Int worldPos) {
        const auto chunkPos = WorldToChunk(worldPos);

        return {
            worldPos.x - chunkPos.x * Data::WorldData::CHUNK_SIZE,
            worldPos.y - chunkPos.y * Data::WorldData::CHUNK_SIZE
        };
    }

    int LocalToIndex(Data::CoreData::Vector2Int localPos) {
        return localPos.y * Data::WorldData::CHUNK_SIZE + localPos.x;
    }

    bool IsInsideWorld(const Data::WorldData::Map& map,
                       Data::CoreData::Vector2Int chunkPos) {

        if (map.worldWidthChunks > 0 &&
            map.worldHeightChunks > 0) {

            if (chunkPos.x < 0 ||
                chunkPos.x >= map.worldWidthChunks ||
                chunkPos.y < 0 ||
                chunkPos.y >= map.worldHeightChunks) {
                return false;
            }
        }

        return true;
    }

    void SetTileBuildingID(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos,
        Data::WorldData::BuildingId id) {

        auto* tile = GetOrCreateTile(map, worldPos);

        if (tile) {
            tile->buildingId = id;
        }
    }

    Data::WorldData::Tile* GetTile(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos) {

        auto* chunk = GetChunkFromWorldPos(map, worldPos);

        if (!chunk) {
            return nullptr;
        }

        return &chunk->tiles[
            LocalToIndex(WorldToLocal(worldPos))
        ];
    }

    Data::WorldData::Tile* GetOrCreateTile(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos) {

        const auto chunkPos = WorldToChunk(worldPos);

        if (!IsInsideWorld(map, chunkPos)) {
            return nullptr;
        }

        auto& chunk = GetOrCreateChunk(map, chunkPos);

        return &chunk.tiles[
            LocalToIndex(WorldToLocal(worldPos))
        ];
    }

    void Initialize(
        Data::WorldData::Map& map,
        uint64_t seed,
        int widthChunks,
        int heightChunks) {

        map.chunks.clear();

        map.worldSeed = seed;
        map.worldWidthChunks = widthChunks;
        map.worldHeightChunks = heightChunks;

        map.nextBuildingId = 1;

        map.buildingCount = 0;
        map.healthCount = 0;
        map.inventoryCount = 0;
        map.productionCount = 0;
        map.extractorCount = 0;
        map.powerProducerCount = 0;
        map.powerConsumerCount = 0;
        map.conveyorCount = 0;
        map.splitterCount = 0;
    }

    Data::WorldData::Chunk* GetChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos) {

        auto it = map.chunks.find(chunkPos);

        if (it == map.chunks.end()) {
            return nullptr;
        }

        return &it->second;
    }

    Data::WorldData::Chunk* GetChunkFromWorldPos(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos) {

        return GetChunk(map, WorldToChunk(worldPos));
    }

    Data::WorldData::Chunk& CreateChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos) {

        auto it = map.chunks.find(chunkPos);

        if (it != map.chunks.end()) {
            return it->second;
        }

        auto& chunk = map.chunks[chunkPos];

        chunk = {};

        chunk.position = chunkPos;
        chunk.state = Data::WorldData::ChunkState::UNLOADED;

        // TODO: Replace default tile initialization with procedural world generation.
        for (int i = 0; i < Data::WorldData::CHUNK_SIZE * Data::WorldData::CHUNK_SIZE; ++i) {
            chunk.tiles[i].floor = Data::WorldData::FloorType::STONE;
            chunk.tiles[i].ore = Data::WorldData::ItemType::NONE;
            chunk.tiles[i].buildingId = -1;
            chunk.tiles[i].isPassable = true;
        }

        return chunk;
    }

    Data::WorldData::Chunk& GetOrCreateChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos) {

        auto it = map.chunks.find(chunkPos);

        if (it != map.chunks.end()) {
            return it->second;
        }

        return CreateChunk(map, chunkPos);
    }

} // namespace Systems::MapSystem