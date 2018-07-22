#pragma once

#include <stdint.h>
#include <string>
#include "glmath.h"
#include "log.h"

struct Obstacle {
	std::string name;
	ivec2 tile;
	uint8_t type;
	uint8_t properties[3];
};

struct Particle {
	vec2 position;
	uint8_t group;
	uint8_t subtype;
	vec2 velocity;
};

struct Object {
	std::string name;
	uint8_t type;
	ivec2 position;
	uint8_t properties[16];
};