#include "../../../include/Systems/World/BuildingSystem.h"

namespace Systems::World::BuildingSystem {
    
    std::vector<Data::WorldData::Building> activeBuildings;
    int nextBuildingID = 1;

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