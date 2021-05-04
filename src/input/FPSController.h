#pragma once 

#include "PlayerController.h"
#include "physics/Controller.h"
#include "physics/System.h"
#include "render/Camera.h"
#include "render/System.h"

namespace input {

class FPSController : public PlayerController {
    physics::Controller* capsule_controller_;
    render::Camera* camera_;

    float delta_walk_[2]; // Forward, Right
    float delta_look_[2]; // X axis, Y axis

public:
    FPSController(physics::Controller* capsule_controller, render::Camera* camera);

    void Register(base::Entity* entity);

    void WalkForward();
    void WalkBack();
    void WalkRight();
    void WalkLeft();

    int Update(float delta_time);
};

};