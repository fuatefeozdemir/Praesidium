#include "../../include/Systems/ConveyorSystem.h"
#include "../../include/Systems/BuildingSystem.h"
#include <algorithm>

namespace {

    using namespace Data::WorldData;

    bool TryInjectItem(ConveyorComponent& conveyor, const Item& item, uint16_t carryOverProgress) {
        if (!Systems::ConveyorSystem::CanAcceptItem(conveyor)) {
            return false;
        }

        uint16_t startingProgress = carryOverProgress;

        // Prevent item clipping: Ensure physical distance from the item ahead.
        if (conveyor.itemCount > 0) {
            int maxAllowed = static_cast<int>(conveyor.items[0].progress) - CONVEYOR_ITEM_SPACING;
            if (startingProgress > maxAllowed) {
                startingProgress = static_cast<uint16_t>(std::max(0, maxAllowed));
            }
        }

        for (int i = conveyor.itemCount; i > 0; --i) {
            conveyor.items[i] = conveyor.items[i - 1];
        }

        conveyor.items[0].item = item;
        conveyor.items[0].progress = startingProgress;
        conveyor.itemCount++;

        return true;
    }

    void AdvanceItems(ConveyorComponent& conveyor) {
        for (int i = conveyor.itemCount - 1; i >= 0; --i) {

            if (i == conveyor.itemCount - 1) {
                if (conveyor.items[i].progress >= CONVEYOR_PROGRESS_PER_TILE) {
                    continue;
                }
                conveyor.items[i].progress += conveyor.speedPerTick;
            }
            else {
                int maxAllowed = std::max(0, static_cast<int>(conveyor.items[i + 1].progress) - CONVEYOR_ITEM_SPACING);
                int newProgress = static_cast<int>(conveyor.items[i].progress) + conveyor.speedPerTick;

                if (newProgress > maxAllowed) {
                    newProgress = maxAllowed;
                }

                if (newProgress < conveyor.items[i].progress) {
                    newProgress = conveyor.items[i].progress;
                }

                conveyor.items[i].progress = static_cast<uint16_t>(newProgress);
            }
        }
    }

    void ProcessTransfers(ConveyorComponent& conveyor, Map& map) {
        if (conveyor.itemCount == 0) {
            return;
        }

        const int frontIndex = conveyor.itemCount - 1;
        ConveyorItem& frontItem = conveyor.items[frontIndex];

        if (frontItem.progress >= CONVEYOR_PROGRESS_PER_TILE) {

            if (conveyor.nextBuildingId == -1) {
                return;
            }

            Building* nextBuilding = Systems::BuildingSystem::GetBuilding(map, conveyor.nextBuildingId);

            if (!nextBuilding || nextBuilding->state != BuildingState::ACTIVE) {
                return;
            }

            bool transferSuccessful = false;

            switch (nextBuilding->type) {
                case BuildingType::CONVEYOR_BELT: {
                    if (nextBuilding->conveyorIndex != -1) {
                        ConveyorComponent& nextConveyor = map.conveyors[nextBuilding->conveyorIndex];
                        uint16_t carryOverProgress = frontItem.progress - CONVEYOR_PROGRESS_PER_TILE;
                        transferSuccessful = TryInjectItem(nextConveyor, frontItem.item, carryOverProgress);
                    }
                    break;
                }
                case BuildingType::FURNACE:
                case BuildingType::COAL_GENERATOR: {
                    // TODO: Implement Machine/Inventory insertion logic
                    break;
                }
                case BuildingType::SPLITTER: {
                    // TODO: Implement Splitter logic
                    break;
                }
                default:
                    break;
            }

            if (transferSuccessful) {
                conveyor.items[frontIndex] = {}; // Clear ghost item data
                conveyor.itemCount--;
            }
        }
    }

} // namespace

namespace Systems::ConveyorSystem {

    using namespace Data::WorldData;

    void Update(Map& map) {
        for (int i = 0; i < map.conveyorCount; ++i) {
            AdvanceItems(map.conveyors[i]);
        }

        for (int i = 0; i < map.conveyorCount; ++i) {
            ProcessTransfers(map.conveyors[i], map);
        }
    }

    bool CanAcceptItem(const ConveyorComponent& conveyor) {
        if (conveyor.itemCount >= MAX_CONVEYOR_ITEMS) {
            return false;
        }

        if (conveyor.itemCount > 0 && conveyor.items[0].progress < CONVEYOR_ITEM_SPACING) {
            return false;
        }

        return true;
    }

    bool PushItem(ConveyorComponent& conveyor, const Item& item) {
        return TryInjectItem(conveyor, item, 0);
    }

} // namespace Systems::ConveyorSystem