#pragma once

#include <vector>

#include "../Data/CoreData/GameContext.h"
#include "../Data/WorldData/MapData.h"

namespace Systems::ChunkSystem {

    // Initializes the chunk management system.
    void Initialize();

    // Updates chunk loading, unloading and visibility.
    void Update(Data::CoreData::GameContext* context);

    // Ensures that the specified chunk is loaded.
    void EnsureChunkLoaded(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPosition
    );

    // Unloads the specified chunk.
    void UnloadChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPosition
    );

    // Returns the list of chunks currently visible by the camera.
    const std::vector<Data::WorldData::Chunk*>& GetVisibleChunks();

    // Returns the list of chunks currently loaded in memory.
    const std::vector<Data::WorldData::Chunk*>& GetLoadedChunks();

} // namespace Systems::ChunkSystem