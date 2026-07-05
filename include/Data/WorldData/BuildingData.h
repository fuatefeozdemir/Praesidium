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
        int currentCraftingTicks = 0;
        // Fabrikaların girdileri ve çıktıları ayrı tutulmalı ki bantlar nereye eşya vereceğini bilsin
        std::map<ItemType, int> inputBuffer;
        std::map<ItemType, int> outputBuffer;
    };

    // 2. Taşıma Bantları İçin
    struct ConveyorComponent {
        int direction; // 0: Kuzey, 1: Doğu, 2: Güney, 3: Batı
        ItemType currentItem = ItemType::NONE;
        int moveProgressTicks = 0;
    };

    // 3. Savunma Binaları İçin (Kuleler)
    struct TurretComponent {
        int range;
        int damage;
        int fireRateCooldownTicks;
        int currentCooldownTicks = 0;
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

        // Üs Envanteri (Sadece CORE_BASE kullanacak)
        std::map<ItemType, int> globalInventory;

        bool isBuilt = false;         // Bina tamamlandı mı? (False ise şantiyedir)
        int buildProgressTicks = 0;
        int maxBuildTicks = 120;      // 2 saniye * 60 tick = 120 tick
        std::map<ItemType, int> remainingCost; // Kalan inşaat maliyeti
        int ticksSinceLastDeduction = 0;

        // BİLEŞENLER (Sadece ilgili bina türlerinde dolu olacak)
        std::optional<FactoryComponent> factoryData;
        std::optional<ConveyorComponent> conveyorData;
        std::optional<TurretComponent> turretData;
    };
}