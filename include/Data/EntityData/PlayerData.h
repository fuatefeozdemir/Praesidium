#pragma once
#include "raylib.h"
#include <string>
#include <vector>

namespace Data::EntityData {

    struct Vector2Int {
        int x;
        int y;
    };

    struct InventorySlot {
        int itemID;
        int count;
    };

    enum class PlayerActionState {
        IDLE,
        MOVING,
        BUILDING
    };

    struct Player {
        // Kimlik
        std::string name;

        // Fizik ve Hareket
        Vector2Int position;
        int speed;
        int collisionRadius;

        // Durum ve İstatistikler
        PlayerActionState state;
        int health;
        int maxHealth;

        // Envanter
        int maxSlots;
        std::vector<InventorySlot> inventory;

        // Etkileşim süresi takibi için
        int actionTimer = 0;
        int lastTargetX = -1;
        int lastTargetY = -1;
    };
}