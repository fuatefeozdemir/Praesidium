#include "../../include/Systems/BuildingSystem.h"
#include "../../include/Systems/MapSystem.h"

namespace Systems::BuildingSystem {

    // ==========================================
    // LOOKUP (ARAMA)
    // ==========================================
    Data::WorldData::Building* GetBuilding(Data::WorldData::Map& map, Data::WorldData::BuildingId id) {
        for (int i = 0; i < map.activeBuildingCount; ++i) {
            if (map.buildings[i].id == id) {
                return &map.buildings[i];
            }
        }
        return nullptr;
    }

    // ==========================================
    // DOĞRULAMA (VALIDATION)
    // ==========================================
    bool CanPlaceBuilding(Data::WorldData::Map& map, const Data::WorldData::BuildingDefinition* def, Data::CoreData::Vector2Int position) {
        for (int y = 0; y < def->defaultSize.y; ++y) {
            for (int x = 0; x < def->defaultSize.x; ++x) {
                Data::CoreData::Vector2Int worldPos = {position.x + x, position.y + y};
                Data::CoreData::Vector2Int chunkPos = MapSystem::WorldToChunk(worldPos);

                // Dünya sınırları kontrolü
                if (!MapSystem::IsInsideWorld(map, chunkPos)) {
                    return false;
                }

                // Chunk bellekte varsa, tile dolu mu kontrol et (Chunk yoksa boştur, yerleştirilebilir)
                Data::WorldData::Tile* tile = MapSystem::GetTile(map, worldPos);
                if (tile && tile->buildingID != -1) {
                    return false; // Çakışma var
                }
            }
        }
        return true;
    }

    // ==========================================
    // ALLOCATOR (TAHSİS EDİCİ)
    // ==========================================
    Data::WorldData::BuildingId CreateBuilding(
        Data::WorldData::Map& map,
        Data::WorldData::BuildingType type,
        Data::CoreData::Vector2Int position,
        Data::WorldData::Direction direction)
    {
        // 1. Şablonu (Definition) Oku
        const auto* def = Data::WorldData::GetBuildingDefinition(type);
        if (!def) return -1; // Geçersiz bina tipi

        // 2. Uzaysal Doğrulama (Collision & Bounds Check)
        if (!CanPlaceBuilding(map, def, position)) {
            return -1; // İnşa edilemez alan
        }

        // 3. Kapasite Ön-Kontrolü (Partial Allocation Koruması)
        if (map.activeBuildingCount >= Data::WorldData::MAX_BUILDINGS) return -1;
        if (def->hasHealth && map.activeHealthCount >= Data::WorldData::MAX_BUILDINGS) return -1;
        if (def->hasInventory && map.activeInventoryCount >= Data::WorldData::MAX_INVENTORIES) return -1;
        if (def->hasProduction && map.activeProductionCount >= Data::WorldData::MAX_PRODUCTIONS) return -1;
        if (def->hasExtractor && map.activeExtractorCount >= Data::WorldData::MAX_EXTRACTORS) return -1;
        if (def->hasPowerProducer && map.activePowerProducerCount >= Data::WorldData::MAX_POWER_NODES) return -1;
        if (def->hasPowerConsumer && map.activePowerConsumerCount >= Data::WorldData::MAX_POWER_NODES) return -1;
        if (def->hasConveyor && map.activeConveyorCount >= Data::WorldData::MAX_CONVEYORS) return -1;
        if (def->hasSplitter && map.activeSplitterCount >= Data::WorldData::MAX_SPLITTERS) return -1;

        // 4. Ana Binayı Tahsis Et
        Data::WorldData::BuildingId newId = map.nextBuildingID++;
        int bIndex = map.activeBuildingCount++;
        auto& building = map.buildings[bIndex];

        building.id = newId;
        building.type = type;
        building.position = position;
        building.size = def->defaultSize;
        building.direction = direction;
        building.state = Data::WorldData::BuildingState::ACTIVE;

        // 5. Componentleri Tahsis Et
        if (def->hasHealth) {
            int cIndex = map.activeHealthCount++;
            map.healths[cIndex].buildingId = newId;
            map.healths[cIndex].currentHealth = def->baseHealth;
            building.healthIndex = cIndex;
        }

        if (def->hasInventory) {
            int cIndex = map.activeInventoryCount++;
            map.inventories[cIndex].buildingId = newId;
            building.inventoryIndex = cIndex;
        }

        if (def->hasProduction) {
            int cIndex = map.activeProductionCount++;
            map.productions[cIndex].buildingId = newId;
            building.productionIndex = cIndex;
        }

        if (def->hasExtractor) {
            int cIndex = map.activeExtractorCount++;
            map.extractors[cIndex].buildingId = newId;
            building.extractorIndex = cIndex;
        }

        if (def->hasPowerProducer) {
            int cIndex = map.activePowerProducerCount++;
            map.powerProducers[cIndex].buildingId = newId;
            building.powerProducerIndex = cIndex;
        }

        if (def->hasPowerConsumer) {
            int cIndex = map.activePowerConsumerCount++;
            map.powerConsumers[cIndex].buildingId = newId;
            building.powerConsumerIndex = cIndex;
        }

        if (def->hasConveyor) {
            int cIndex = map.activeConveyorCount++;
            map.conveyors[cIndex].buildingId = newId;
            building.conveyorIndex = cIndex;
        }

        if (def->hasSplitter) {
            int cIndex = map.activeSplitterCount++;
            map.splitters[cIndex].buildingId = newId;
            building.splitterIndex = cIndex;
        }

        // 6. Harita Üzerine Yerleştir ve Grid'i İşgal Et
        for (int y = 0; y < def->defaultSize.y; ++y) {
            for (int x = 0; x < def->defaultSize.x; ++x) {
                Data::CoreData::Vector2Int worldPos = {position.x + x, position.y + y};
                MapSystem::SetTileBuildingID(map, worldPos, newId);
            }
        }

        return newId;
    }

    // ==========================================
    // DEALLOCATOR (SİLİCİ)
    // ==========================================
    void DestroyBuilding(Data::WorldData::Map& map, Data::WorldData::BuildingId id) {
        // İleride eklenecek
    }
}