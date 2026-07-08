#pragma once
#include "../Data/WorldData/MapData.h"
#include "../Data/CoreData/Vector2Int.h"

namespace Systems::MapSystem {

    // ==========================================
    // KOORDİNAT VE YARDIMCI FONKSİYONLAR
    // ==========================================

    // Dünya koordinatını Chunk koordinatına çevirir (Negatif eksen destekli).
    Data::CoreData::Vector2Int WorldToChunk(Data::CoreData::Vector2Int worldPos);

    // Dünya koordinatının, bulunduğu Chunk içindeki yerel X ve Y (0-31) karşılığını verir.
    Data::CoreData::Vector2Int WorldToLocal(Data::CoreData::Vector2Int worldPos);

    // Yerel X ve Y koordinatını, tek boyutlu Tile dizisi için indekse çevirir.
    int LocalToIndex(Data::CoreData::Vector2Int localPos);

    // Verilen Chunk koordinatının, belirlenmiş dünya sınırları içinde olup olmadığını kontrol eder.
    bool IsInsideWorld(const Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos);

    // ==========================================
    // TILE ERİŞİM API
    // ==========================================

    // Belirtilen dünya koordinatındaki Tile'ın bina ID'sini günceller. Chunk yoksa güvenli bir şekilde üretir.
    void SetTileBuildingID(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos, Data::WorldData::BuildingId id);

    // Varsa Tile döndürür, yoksa (veya sınır dışıysa) nullptr döner. Sadece okuma işlemleri içindir.
    Data::WorldData::Tile* GetTile(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos);

    // Tile'ı kesin olarak döndürür. Chunk yoksa yeni üretir. Sınır dışıysa nullptr döner. Yazma işlemleri içindir.
    Data::WorldData::Tile* GetOrCreateTile(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos);

    // ==========================================
    // CHUNK YÖNETİM API
    // ==========================================

    // Haritayı ve tüm component havuzu sayaçlarını sıfırlar. Yeniden başlatma ve yükleme (Load) için kullanılır.
    void Initialize(Data::WorldData::Map& map, uint64_t seed, int widthChunks = 0, int heightChunks = 0);

    // Bellekte varsa Chunk referansını döndürür, yoksa nullptr döner.
    Data::WorldData::Chunk* GetChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos);
    Data::WorldData::Chunk* GetChunkFromWorldPos(Data::WorldData::Map& map, Data::CoreData::Vector2Int worldPos);

    // Chunk'ı sıfırdan oluşturur ve Hash Map'e ekler. Varsa üzerine yazmaz, mevcut olanı döndürür.
    Data::WorldData::Chunk& CreateChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos);

    // Oyun döngüsünde en çok kullanılan metod: Chunk varsa getirir, yoksa üretip getirir.
    Data::WorldData::Chunk& GetOrCreateChunk(Data::WorldData::Map& map, Data::CoreData::Vector2Int chunkPos);
}