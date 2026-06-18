#pragma once
#include <vector>
#include <map>
#include <optional>
#include "ItemData.h"

namespace Data::WorldData {

    enum class BuildingType {
        NONE, CORE_BASE, WALL, CONVEYOR, FACTORY, TURRET
    };

    // 1. Üretim Yapan Binalar İçin (Fabrikalar, Fırınlar)
    struct FactoryComponent {
        int activeRecipeID = -1;
        float currentCraftingProgress = 0.0f;
        // Fabrikaların girdileri ve çıktıları ayrı tutulmalı ki bantlar nereye eşya vereceğini bilsin
        std::map<ItemType, int> inputBuffer;
        std::map<ItemType, int> outputBuffer;
    };

    // 2. Taşıma Bantları İçin
    struct ConveyorComponent {
        int direction; // 0: Kuzey, 1: Doğu, 2: Güney, 3: Batı
        ItemType currentItem = ItemType::NONE;
        float moveProgress = 0.0f; // Eşyanın bandın üzerindeki ilerleyişi (0.0 - 1.0)
    };

    // 3. Savunma Binaları İçin (Kuleler)
    struct TurretComponent {
        float range;
        int damage;
        float fireRateCooldown;
        float currentCooldown = 0.0f;
        // Eğer mermi kullanıyorsa kendi küçük envanteri
        std::map<ItemType, int> ammoStorage;
    };

    // ANA BİNA YAPISI
    struct Building {
        int id;
        BuildingType type;

        // Ortak Fiziksel Özellikler
        int gridX, gridY;
        int width, height;
        int health;
        int maxHealth;

        // Genel Envanter (Özellikle CORE_BASE için devasa depo)
        std::map<ItemType, int> globalInventory;

        // BİLEŞENLER (Sadece ilgili bina türlerinde dolu olacak)
        std::optional<FactoryComponent> factoryData;
        std::optional<ConveyorComponent> conveyorData;
        std::optional<TurretComponent> turretData;
    };
}