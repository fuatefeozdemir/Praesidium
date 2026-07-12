#pragma once

#include <cstdint>

#include "../CoreData/Vector2Int.h"
#include "ItemData.h"

namespace Data::WorldData {

    using BuildingId = int;

    enum class BuildingType {
        NONE = 0,
        WALL,
        MINER,
        FURNACE,
        CONVEYOR_BELT,
        SPLITTER,
        COAL_GENERATOR
        // TODO: Expand with additional buildings.
    };

    enum class BuildingState {
        ACTIVE,
        DISABLED,
        NO_POWER,
        NO_INPUT,
        BROKEN
    };

    enum class Direction {
        NORTH,
        EAST,
        SOUTH,
        WEST
    };

    // Static Building Definitions

    struct BuildingDefinition {
        BuildingType type;
        CoreData::Vector2Int defaultSize;
        int baseHealth;
        int maxTier;

        // Component Flags
        bool hasHealth;
        bool hasInventory;
        bool hasProduction;
        bool hasExtractor;
        bool hasPowerProducer;
        bool hasPowerConsumer;
        bool hasConveyor;
        bool hasSplitter;
    };

    // TODO: Load building definitions from external data.

    inline constexpr BuildingDefinition BuildingRegistry[] = {
        // Type,                       Size,   HP,   Tier, Health, Inv,   Prod,  Ext,   PwrProd, PwrCons, Conv,  Split
        { BuildingType::WALL,          {1, 1}, 500,  1,    true,   false, false, false, false,   false,   false, false },
        { BuildingType::MINER,         {2, 2}, 200,  1,    true,   true,  false, true,  false,   true,    false, false },
        { BuildingType::FURNACE,       {2, 2}, 300,  1,    true,   true,  true,  false, false,   true,    false, false },
        { BuildingType::CONVEYOR_BELT, {1, 1}, 50,   1,    true,   false, false, false, false,   false,   true,  false },
        { BuildingType::SPLITTER,      {1, 1}, 100,  1,    true,   false, false, false, false,   false,   false, true  },
        { BuildingType::COAL_GENERATOR,{3, 3}, 400,  1,    true,   true,  false, false, true,    false,   false, false }
    };

    inline const BuildingDefinition* GetBuildingDefinition(BuildingType type) {
        for (const auto& definition : BuildingRegistry) {
            if (definition.type == type) {
                return &definition;
            }
        }

        return nullptr;
    }

    // Building Instance

    struct Building {
        BuildingId id = 0;
        BuildingType type = BuildingType::NONE;
        int tier = 1;
        BuildingState state = BuildingState::ACTIVE;

        CoreData::Vector2Int position = {0, 0};
        CoreData::Vector2Int size = {1, 1};
        Direction direction = Direction::NORTH;

        // Component Indices (-1 = Not Assigned)
        int inventoryIndex = -1;
        int productionIndex = -1;
        int extractorIndex = -1;
        int powerProducerIndex = -1;
        int powerConsumerIndex = -1;
        int conveyorIndex = -1;
        int splitterIndex = -1;
        int healthIndex = -1;
    };

    // Components

    struct HealthComponent {
        BuildingId buildingId = -1;
        int currentHealth = 0;
    };

    struct InventoryComponent {
        BuildingId buildingId = -1;
        int items[static_cast<int>(ItemType::MAX_ITEM_COUNT)] = {0};
    };

    struct ProductionComponent {
        BuildingId buildingId = -1;
        int currentRecipeId = -1;
        int currentTick = 0;
        bool isProducing = false;
    };

    struct ExtractorComponent {
        BuildingId buildingId = -1;
        ItemType targetOre = ItemType::NONE;
        int ticksPerMine = 0;
        int currentTick = 0;
    };

    struct PowerProducerComponent {
        BuildingId buildingId = -1;
        int generationRate = 0;
        bool isActive = false;
    };

    struct PowerConsumerComponent {
        BuildingId buildingId = -1;
        int consumptionRate = 0;
        int storedEnergy = 0;
        bool isPowered = false;
    };

    constexpr int MAX_CONVEYOR_ITEMS = 4;
    constexpr int CONVEYOR_PROGRESS_PER_TILE = 1024;

    struct ConveyorItem {
        Item item;
        uint16_t progress = 0; // 0-1024
    };

    struct ConveyorComponent {
        BuildingId buildingId = -1;

        BuildingId previousBuildingId = -1;
        BuildingId nextBuildingId = -1;

        ConveyorItem items[MAX_CONVEYOR_ITEMS];
        int itemCount = 0;

        int speedPerTick = 16;
    };

    struct SplitterComponent {
        BuildingId buildingId = -1;
        int outputCounter = 0;
    };

} // namespace Data::WorldData