#include "../../../include/Systems/Simulation/PlayerSystem.h"
#include "raylib.h"

namespace Systems::Simulation::PlayerSystem {
    void Update(Data::EntityData::Player& player) {
        float dt = GetFrameTime();
        bool isMoving = false;

        if (IsKeyDown(KEY_W)) { 
            player.position.y -= player.speed * dt; 
            isMoving = true; 
        }
        if (IsKeyDown(KEY_S)) { 
            player.position.y += player.speed * dt; 
            isMoving = true; 
        }
        if (IsKeyDown(KEY_A)) { 
            player.position.x -= player.speed * dt; 
            isMoving = true; 
        }
        if (IsKeyDown(KEY_D)) { 
            player.position.x += player.speed * dt; 
            isMoving = true; 
        }

        // Karakterin durumunu (State) güncelle
        if (isMoving) {
            player.state = Data::EntityData::PlayerActionState::MOVING;
        } else if (player.state != Data::EntityData::PlayerActionState::BUILDING) {
            // Eğer inşaat yapmıyorsa ve hareket de etmiyorsa IDLE'a çek
            player.state = Data::EntityData::PlayerActionState::IDLE;
        }
    }
}