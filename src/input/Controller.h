#pragma once 
#include "base/Entity.h"

namespace input {

enum ButtonEvent {
    WALK_FORWARD,
    WALK_BACK,
    WALK_RIGHT,
    WALK_LEFT
};

enum AxisEvent{
    MOUSE_X,
    MOUSE_Y,
};

class Controller {
public:
    virtual void Register(base::Entity* entity) = 0;

    virtual int Update(float delta_time) = 0;
};

};