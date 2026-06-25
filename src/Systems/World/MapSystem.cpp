#include "../../../include/Systems/World/MapSystem.h"
#include "raylib.h"
#include "../../../include/Systems/World/BuildingSystem.h"

namespace Systems::World::MapSystem {

    // Yardımcı Fonksiyon: Belirli bir bölgeye dairesel maden öbeği yerleştirir
    void GenerateOrePatch(Data::WorldData::Map& map, Data::WorldData::OreType type, int count) {
        for (int i = 0; i < count; i++) {
            int centerX = GetRandomValue(0, map.width - 1);
            int centerY = GetRandomValue(0, map.height - 1);
            int radius = GetRandomValue(2, 5); // Öbek büyüklüğü (2-5 hücre yarıçapı)

            for (int y = centerY - radius; y <= centerY + radius; y++) {
                for (int x = centerX - radius; x <= centerX + radius; x++) {
                    if (x >= 0 && x < map.width && y >= 0 && y < map.height) {
                        // Daire şeklinde bir dağılım için mesafe hesabı
                        if ((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY) <= radius * radius) {
                            int index = y * map.width + x;
                            map.tiles[index].ore = type;
                        }
                    }
                }
            }
        }
    }

    void Initialize(Data::WorldData::Map& map, int width, int height, int tileSize) {
        map.width = width;
        map.height = height;
        map.tileSize = tileSize;
        map.tiles.resize(width * height);

        // 1. Tüm haritayı varsayılan taş zemin ile doldur
        for (int i = 0; i < width * height; i++) {
            map.tiles[i].floor = Data::WorldData::FloorType::STONE;
            map.tiles[i].ore = Data::WorldData::OreType::NONE;
            map.tiles[i].buildingID = -1;
            map.tiles[i].isPassable = true;
        }

        // 2. Rastgele maden öbekleri oluştur
        GenerateOrePatch(map, Data::WorldData::OreType::IRON, 15);   // 15 adet demir bölgesi
        GenerateOrePatch(map, Data::WorldData::OreType::COPPER, 12); // 12 adet bakır bölgesi
        GenerateOrePatch(map, Data::WorldData::OreType::COAL, 10);   // 10 adet kömür bölgesi

        Systems::World::BuildingSystem::CreateCoreBase(map);
    }
}
