#pragma once

#include <ssm/matrix.hpp>

namespace ecs {
struct Camera {
	ssm::mat4 view, projection;
};
}
