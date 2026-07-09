#pragma once

#include <cstdint>
#include <unordered_map>

#include "../CoreData/Vector2Int.h"
#include "BuildingData.h"
#include "ItemData.h"

namespace Data::WorldData {

    // Memory Limits

    constexpr int CHUNK_SIZE = 128;

    constexpr int MAX_BUILDINGS = 65536;
    constexpr int MAX_INVENTORIES = 30000;
    constexpr int MAX_PRODUCTIONS = 15000;
    constexpr int MAX_EXTRACTORS = 10000;
    constexpr int MAX_POWER_COMPONENTS = 40000;
    constexpr int MAX_CONVEYORS = 50000;
    constexpr int MAX_SPLITTERS = 10000;

    enum class FloorType : uint8_t {
        STONE,
        DIRT,
        SAND,
        WATER
    };

    // Spatial Data

    struct Tile {
        FloorType floor = FloorType::STONE;
        ItemType ore = ItemType::NONE;
        BuildingId buildingId = -1;
        bool isPassable = true;
    };

    struct Chunk {
        CoreData::Vector2Int position;

        bool isLoaded = false;
        bool isModified = false;
        bool needsSimulation = false;

        Tile tiles[CHUNK_SIZE * CHUNK_SIZE];
    };

    struct Vector2IntHash {
        std::size_t operator()(const CoreData::Vector2Int& v) const noexcept {
            return (static_cast<uint64_t>(static_cast<uint32_t>(v.x)) << 32) |
                   static_cast<uint32_t>(v.y);
        }
    };

    // World Map

    struct Map {
        // World Information

        uint64_t worldSeed = 0;
        int worldWidthChunks = 0;
        int worldHeightChunks = 0;

        // Loaded Chunks

        std::unordered_map<CoreData::Vector2Int, Chunk, Vector2IntHash> chunks;

        // ECS Storage

        int nextBuildingId = 1;

        Building buildings[MAX_BUILDINGS];
        int buildingCount = 0;

        HealthComponent healths[MAX_BUILDINGS];
        int healthCount = 0;

        InventoryComponent inventories[MAX_INVENTORIES];
        int inventoryCount = 0;

        ProductionComponent productions[MAX_PRODUCTIONS];
        int productionCount = 0;

        ExtractorComponent extractors[MAX_EXTRACTORS];
        int extractorCount = 0;

        PowerProducerComponent powerProducers[MAX_POWER_COMPONENTS];
        int powerProducerCount = 0;

        PowerConsumerComponent powerConsumers[MAX_POWER_COMPONENTS];
        int powerConsumerCount = 0;

        ConveyorComponent conveyors[MAX_CONVEYORS];
        int conveyorCount = 0;

        SplitterComponent splitters[MAX_SPLITTERS];
        int splitterCount = 0;
    };

} // namespace Data::WorldData