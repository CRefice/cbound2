#pragma once

#include <ssm\vec3.hpp>
#include <ssm\quat.hpp>

struct Transform
{
	ssm::vec3 pos;
	ssm::quat rot;
	ssm::vec3 scale;
};
