#pragma once

namespace Data::CoreData {

    struct Vector2Int {
        int x;
        int y;

        // Comparison operators
        bool operator==(const Vector2Int& other) const {
            return x == other.x && y == other.y;
        }

        bool operator!=(const Vector2Int& other) const {
            return !(*this == other);
        }
    };

}