#include "../../include/Systems/ChunkSystem.h"

#include <algorithm>
#include <cstdlib>

#include "raylib.h"

#include "../../include/Engine/Config/WorldConfig.h"
#include "../../include/Systems/CameraSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace {

    constexpr int PRELOAD_DISTANCE = 2;
    constexpr int UNLOAD_DISTANCE = 5;

    // Cached chunk lists used to avoid per-frame allocations.
    std::vector<Data::WorldData::Chunk*> visibleChunksCache;
    std::vector<Data::WorldData::Chunk*> loadedChunksCache;

    // Current chunk containing the camera.
    Data::CoreData::Vector2Int currentCameraChunk = {0, 0};

    void UpdateCurrentCameraChunk() {

        const Vector2 cameraPosition =
            Systems::CameraSystem::GetCamera().target;

        currentCameraChunk =
            Systems::MapSystem::WorldToChunk({
                static_cast<int>(cameraPosition.x),
                static_cast<int>(cameraPosition.y)
            });
    }

    void RebuildVisibleChunkCache() {

        visibleChunksCache.clear();

        // TODO: Determine which loaded chunks intersect the camera view
        // and rebuild the visible chunk cache.

        const Rectangle viewBounds =
            Systems::CameraSystem::GetViewBounds();

        const float chunkPixelSize = static_cast<float>(
            Data::WorldData::CHUNK_SIZE *
            Engine::Config::TILE_SIZE
        );

        for (auto* chunk : loadedChunksCache) {

            const Rectangle chunkBounds = {
                static_cast<float>(chunk->position.x) * chunkPixelSize,
                static_cast<float>(chunk->position.y) * chunkPixelSize,
                chunkPixelSize,
                chunkPixelSize
            };

            if (CheckCollisionRecs(viewBounds, chunkBounds)) {
                chunk->state = Data::WorldData::ChunkState::VISIBLE;
                visibleChunksCache.push_back(chunk);
            } else {
                chunk->state = Data::WorldData::ChunkState::LOADED;
            }
        }
    }

    void UnloadDistantChunks(Data::WorldData::Map& map) {

        // TODO: Unload chunks outside the configured streaming distance.

        loadedChunksCache.erase(
            std::remove_if(
                loadedChunksCache.begin(),
                loadedChunksCache.end(),
                [&map](Data::WorldData::Chunk* chunk) {

                    const int distanceX =
                        std::abs(chunk->position.x - currentCameraChunk.x);

                    const int distanceY =
                        std::abs(chunk->position.y - currentCameraChunk.y);

                    if (distanceX > UNLOAD_DISTANCE ||
                        distanceY > UNLOAD_DISTANCE) {

                        Systems::ChunkSystem::UnloadChunk(
                            map,
                            chunk->position
                        );

                        return true;
                    }

                    return false;
                }
            ),
            loadedChunksCache.end()
        );
    }

} // namespace

namespace Systems::ChunkSystem {

    void Initialize() {

        const int preloadDiameter =
            PRELOAD_DISTANCE * 2 + 1;

        loadedChunksCache.reserve(
            preloadDiameter * preloadDiameter
        );

        visibleChunksCache.reserve(
            preloadDiameter * preloadDiameter
        );
    }

    void Update(Data::CoreData::GameContext* context) {

        auto& map = context->worldMap;

        UpdateCurrentCameraChunk();

        // Load all chunks inside the preload area.
        for (int y = -PRELOAD_DISTANCE; y <= PRELOAD_DISTANCE; ++y) {
            for (int x = -PRELOAD_DISTANCE; x <= PRELOAD_DISTANCE; ++x) {

                const Data::CoreData::Vector2Int chunkPosition = {
                    currentCameraChunk.x + x,
                    currentCameraChunk.y + y
                };

                EnsureChunkLoaded(
                    map,
                    chunkPosition
                );
            }
        }

        UnloadDistantChunks(map);

        RebuildVisibleChunkCache();
    }

    void EnsureChunkLoaded(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPosition) {

        auto& chunk =
            Systems::MapSystem::GetOrCreateChunk(
                map,
                chunkPosition
            );

        if (chunk.state != Data::WorldData::ChunkState::UNLOADED) {
            return;
        }

        // TODO: Restore chunk data from a save file
        // or generate it procedurally.

        chunk.state = Data::WorldData::ChunkState::LOADED;

        loadedChunksCache.push_back(&chunk);
    }

    void UnloadChunk(
        Data::WorldData::Map& map,
        Data::CoreData::Vector2Int chunkPosition) {

        // TODO: Save modified chunk data before unloading.
        // TODO: Release tile, ore and render cache data.
        // Do not modify simulation data.

        map.chunks.erase(chunkPosition);
    }

    const std::vector<Data::WorldData::Chunk*>& GetVisibleChunks() {
        return visibleChunksCache;
    }

    const std::vector<Data::WorldData::Chunk*>& GetLoadedChunks() {
        return loadedChunksCache;
    }

} // namespace Systems::ChunkSystem