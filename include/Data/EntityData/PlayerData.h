#pragma once
#include "raylib.h"
#include <string>
#include <vector>

namespace Data::EntityData {

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
        Vector2 position;
        float speed;
        float collisionRadius;

        // Durum ve İstatistikler
        PlayerActionState state;
        float health;
        float maxHealth;

        // Envanter
        int maxSlots;
        std::vector<InventorySlot> inventory;

        // Etkileşim süresi takibi için
        float actionTimer = 0.0f;
        int lastTargetX = -1;
        int lastTargetY = -1;
    };
}