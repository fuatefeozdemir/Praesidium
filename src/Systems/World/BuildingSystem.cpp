#include "../../../include/Systems/World/BuildingSystem.h"

namespace Systems::World::BuildingSystem {
    
    std::vector<Data::WorldData::Building> activeBuildings;
    int nextBuildingID = 1;

    Data::WorldData::Building* GetBuilding(int id) {
        for (auto& building : activeBuildings) {
            if (building.id == id) return &building;
        }
        return nullptr;
    }

    Data::WorldData::Building* GetCoreBase() {
        for (auto& building : activeBuildings) {
            if (building.type == Data::WorldData::BuildingType::CORE_BASE) return &building;
        }
        return nullptr;
    }

    void CreateGhostBuilding(Data::WorldData::Map& map, Data::WorldData::BuildingType type, int gridX, int gridY) {
        Data::WorldData::Building ghost;
        ghost.id = nextBuildingID++;
        ghost.type = type;
        ghost.isBuilt = false;
        ghost.buildProgress = 0.0f;
        ghost.maxBuildTime = 2.0f;
        ghost.timeSinceLastDeduction = 0.0f;

        // Boyutlandırma (Geçici olarak 1x1, türlere göre ayrılacak)
        ghost.width = 1;
        ghost.height = 1;
        ghost.gridX = gridX;
        ghost.gridY = gridY;

        // Geçici test maliyeti: 1 Konveyör = 2 Demir (IRON_ORE) istesin
        if (type == Data::WorldData::BuildingType::CONVEYOR) {
            ghost.remainingCost[Data::WorldData::ItemType::IRON_ORE] = 2;
        }

        activeBuildings.push_back(ghost);
        int index = gridY * map.width + gridX;
        map.tiles[index].buildingID = ghost.id;
    }

    void CreateCoreBase(Data::WorldData::Map& map) {
        int centerX = map.width / 2;
        int centerY = map.height / 2;

        Data::WorldData::Building coreBase;
        coreBase.id = nextBuildingID++;
        coreBase.type = Data::WorldData::BuildingType::CORE_BASE;
        coreBase.width = 3;
        coreBase.height = 3;
        coreBase.gridX = centerX - 1;
        coreBase.gridY = centerY - 1;
        coreBase.maxHealth = 1000;
        coreBase.health = 1000;

        activeBuildings.push_back(coreBase);

        for (int y = coreBase.gridY; y < coreBase.gridY + coreBase.height; y++) {
            for (int x = coreBase.gridX; x < coreBase.gridX + coreBase.width; x++) {
                if (x >= 0 && x < map.width && y >= 0 && y < map.height) {
                    int index = y * map.width + x;
                    map.tiles[index].buildingID = coreBase.id;
                    map.tiles[index].ore = Data::WorldData::OreType::NONE; 
                }
            }
        }
    }
}