#pragma once

#include "../Data/WorldData/BuildingData.h"
#include "../Data/WorldData/MapData.h"

namespace Systems::ConveyorSystem {

    void Update(Data::WorldData::Map& map);
    bool PushItem(Data::WorldData::ConveyorComponent& conveyor, const Data::WorldData::Item& item);
    bool CanAcceptItem(const Data::WorldData::ConveyorComponent& conveyor);

} // namespace Systems::ConveyorSystem