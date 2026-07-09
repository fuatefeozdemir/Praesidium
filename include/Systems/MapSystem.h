#pragma once

#include "../Data/CoreData/Vector2Int.h"
#include "../Data/WorldData/MapData.h"

namespace Systems::MapSystem {

    // Coordinate conversion

    Data::CoreData::Vector2Int PixelToTile(int pixelX, int pixelY);
    Data::CoreData::Vector2Int WorldToChunk(Data::CoreData::Vector2Int worldPos);
    Data::CoreData::Vector2Int WorldToLocal(Data::CoreData::Vector2Int worldPos);

    int LocalToIndex(Data::CoreData::Vector2Int localPos);

    bool IsInsideWorld(const Data::WorldData::Map& map,
                       Data::CoreData::Vector2Int chunkPos);

    // Tile access

    void SetTileBuildingID(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos,
        Data::WorldData::BuildingId id
    );

    Data::WorldData::Tile* GetTile(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos
    );

    Data::WorldData::Tile* GetOrCreateTile(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos
    );

    // Chunk management

    void Initialize(
        Data::WorldData::Map& map,
        uint64_t seed,
        int widthChunks = 0,
        int heightChunks = 0
    );

    Data::WorldData::Chunk* GetChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos
    );

    Data::WorldData::Chunk* GetChunkFromWorldPos(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int worldPos
    );

    Data::WorldData::Chunk& CreateChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos
    );

    Data::WorldData::Chunk& GetOrCreateChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPos
    );

} // namespace Systems::MapSystem