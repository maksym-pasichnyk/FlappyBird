#include "component.h"
#include "game_object.h"

Transform2D *Component::transform() {
    return gameObject->transform;
}