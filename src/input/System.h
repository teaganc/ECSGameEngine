#pragma once 

#include "base/Component.h"
#include "base/Engine.h"
#include "base/System.h"

#include "input/Component.h"

#include "physics/Controller.h"
#include "render/Camera.h"

#include "Controller.h"

#include <unordered_map>

namespace input {

class PlayerController;

class System : public base::System {
private: 
    PlayerController* player_controller_;

    base::Engine* engine_;

    std::unordered_map<SDL_Keycode, ButtonEvent> key_map_;

    int Poll();
public:

    System(base::Engine* engine);

    void RegisterComponent(base::Component* component);
    void UnRegisterComponent(base::Component* component);

    int Start();
    int End();
    int Update(double delta_time);

    std::unique_ptr<Component> CreateFPSComponent(physics::Controller* controller, render::Camera* camera);
};

};