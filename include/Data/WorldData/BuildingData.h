#pragma once
#include "raylib.h"
#include "ItemData.h"
#include "RecipeData.h"
#include "../CoreData/Vector2Int.h"

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
        // İleride genişletilecek
    };

    enum class BuildingState {
        ACTIVE,
        DISABLED,
        NO_POWER,
        NO_INPUT,
        BROKEN
    };

    enum class Direction {
        NORTH, EAST, SOUTH, WEST
    };

    // ==========================================
    // ANA YAPI
    // ==========================================
    struct Building {
        BuildingId id = 0;
        BuildingType type = BuildingType::NONE;
        int tier = 1;
        BuildingState state = BuildingState::ACTIVE;

        CoreData::Vector2Int position = {0, 0};
        CoreData::Vector2Int size = {1, 1};
        Direction direction = Direction::NORTH;

        // Component İndeksleri (-1 = Yok)
        int inventoryIndex = -1;
        int productionIndex = -1;
        int extractorIndex = -1;
        int powerProducerIndex = -1;
        int powerConsumerIndex = -1;
        int conveyorIndex = -1;
        int splitterIndex = -1;
        int healthIndex = -1;
    };

    // ==========================================
    // COMPONENTS
    // ==========================================

    struct HealthComponent {
        BuildingId buildingId = -1;
        int currentHealth = 0;
        // maxHealth verisi BuildingType'a bağlı olarak ayrı bir konfigürasyondan okunacak
    };

    struct InventoryComponent {
        BuildingId buildingId = -1;
        int items[static_cast<int>(ItemType::MAX_ITEM_COUNT)] = {0};
    };

    struct ProductionComponent {
        BuildingId buildingId = -1;
        int currentRecipeId = -1;
        int currentTick = 0;
        // targetTicks silindi. Her tick RecipeDatabase'den (recipe.processingTime) çekilecek.
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

    struct ConveyorComponent {
        BuildingId buildingId = -1;
        ItemType currentItem = ItemType::NONE;
        int progressTick = 0;
        int ticksToMove = 0;
    };

    struct SplitterComponent {
        BuildingId buildingId = -1;
        int outputCounter = 0;
    };
}