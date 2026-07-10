#include "../../../include/Interface/World/PlayerRenderer.h"

#include "raylib.h"

namespace {

    constexpr float PLAYER_RADIUS = 12.0f;

    Vector2 ToVector2(const Data::CoreData::Vector2Int& position) {
        return {
            static_cast<float>(position.x),
            static_cast<float>(position.y)
        };
    }

} // namespace

namespace Interface::World::PlayerRenderer {

    void Draw(const Data::EntityData::Player& player) {
        // TODO: Replace basic shape with sprite or animation rendering.
        DrawCircleV(ToVector2(player.position), PLAYER_RADIUS, BLUE);
    }

} // namespace Interface::World::PlayerRenderer