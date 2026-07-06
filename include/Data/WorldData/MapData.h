#pragma once
#include <vector>
#include <cstdint>
#include <unordered_map>
#include "BuildingData.h"
#include "ItemData.h"
#include "../CoreData/Vector2Int.h"

namespace Data::WorldData {

    // ==========================================
    // BELLEK LİMİTLERİ (RAM Optimizasyonu)
    // ==========================================
    constexpr int CHUNK_SIZE = 32;
    constexpr int MAX_BUILDINGS = 65536;
    constexpr int MAX_INVENTORIES = 30000;
    constexpr int MAX_PRODUCTIONS = 15000;
    constexpr int MAX_EXTRACTORS = 10000;
    constexpr int MAX_POWER_NODES = 40000; // Producer + Consumer
    constexpr int MAX_CONVEYORS = 50000;
    constexpr int MAX_SPLITTERS = 10000;

    enum class FloorType : uint8_t {
        STONE,
        DIRT,
        SAND,
        WATER
    };

    // ==========================================
    // UZAYSAL VERİ (SPATIAL DATA)
    // ==========================================

    struct Tile {
        FloorType floor = FloorType::STONE;
        ItemType ore = ItemType::NONE;
        BuildingId buildingID = -1;
        bool isPassable = true;
    };

    struct Chunk {
        CoreData::Vector2Int position;
        Tile tiles[CHUNK_SIZE * CHUNK_SIZE];
        bool isLoaded = false;
        bool isModified = false;
        bool needsSimulation = false;
    };

    struct Vector2IntHash {
        std::size_t operator()(const CoreData::Vector2Int& v) const noexcept {
            return (static_cast<uint64_t>(static_cast<uint32_t>(v.x)) << 32) | static_cast<uint32_t>(v.y);
        }
    };

    // ==========================================
    // ANA HARİTA VE BELLEK HAVUZLARI
    // ==========================================

    struct Map {
        // Dünya Meta Verileri
        uint64_t worldSeed = 0;
        int worldWidthChunks = 0;
        int worldHeightChunks = 0;

        std::unordered_map<CoreData::Vector2Int, Chunk, Vector2IntHash> chunks;
        int nextBuildingID = 1;

        // BİNA VE COMPONENT HAVUZLARI
        Building buildings[MAX_BUILDINGS];
        int activeBuildingCount = 0;

        HealthComponent healths[MAX_BUILDINGS];
        int activeHealthCount = 0;

        InventoryComponent inventories[MAX_INVENTORIES];
        int activeInventoryCount = 0;

        ProductionComponent productions[MAX_PRODUCTIONS];
        int activeProductionCount = 0;

        ExtractorComponent extractors[MAX_EXTRACTORS];
        int activeExtractorCount = 0;

        PowerProducerComponent powerProducers[MAX_POWER_NODES];
        int activePowerProducerCount = 0;

        PowerConsumerComponent powerConsumers[MAX_POWER_NODES];
        int activePowerConsumerCount = 0;

        ConveyorComponent conveyors[MAX_CONVEYORS];
        int activeConveyorCount = 0;

        SplitterComponent splitters[MAX_SPLITTERS];
        int activeSplitterCount = 0;
    };
}