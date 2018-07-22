#include "game_object.h"
#include "transform.h"

GameObject::GameObject() {
    Scene::gameObjects.push_back(this);

    transform = AddComponent<Transform2D>();
}

GameObject::~GameObject() {
    Scene::gameObjects.erase(std::find(Scene::gameObjects.begin(), Scene::gameObjects.end(), this));
}
