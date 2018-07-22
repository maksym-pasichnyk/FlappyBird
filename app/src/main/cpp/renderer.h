#pragma once

#include "component.h"
#include "material.h"

struct Renderer : public Component {
    virtual void render(const Material&) = 0;
};