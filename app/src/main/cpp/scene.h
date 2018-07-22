#pragma once

#include <vector>

struct GameObject;

struct Scene {
    static std::vector<GameObject*> gameObjects;
};
