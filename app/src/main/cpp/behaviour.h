#pragma once

#include "component.h"

struct Behaviour : public Component {
	virtual void update(double) = 0;
};