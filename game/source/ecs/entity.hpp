#pragma once

#include <cstdint>

#include "common/slot-map.hpp"

namespace ecs {
// Since each entity has a position, use its slot-map index as Id
using EntityId = SlotMapHandle;
}
