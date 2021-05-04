#include "System.h"
#include "PlayerController.h"

#include "FPSController.h"

namespace input {

int System::Poll() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                engine_->is_running = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    engine_->is_running = false;
                }
                if (player_controller_ == nullptr) {
                    return 0;
                }
                auto button_event = key_map_.find(event.key.keysym.sym);
                if (button_event != key_map_.end()) {
                    auto button_function = player_controller_->button_binds_.find(button_event->second);
                        if (button_function != player_controller_->button_binds_.end()) {
                            button_function->second();
                        }
                }
                break;
        }
    }
    return 0;
}

System::System(base::Engine* engine) : engine_(engine) {
    key_map_ = {
        {SDLK_w, WALK_FORWARD},        
        {SDLK_a, WALK_LEFT},
        {SDLK_s, WALK_BACK},
        {SDLK_d, WALK_RIGHT}
    };
};

void System::RegisterComponent(base::Component* component) {
// might need to call component->controller-> register or get rid of that function
    auto controller = static_cast<Component*>(component)->controller_.get();
    if (dynamic_cast<PlayerController*>(controller) != nullptr) {
        player_controller_ = static_cast<PlayerController*>(controller);
    }
}
void System::UnRegisterComponent(base::Component* component) {}

int System::Start() { return 0; }
int System::End() { return 0; }

int System::Update(double delta_time) { 
    Poll(); 
    if(player_controller_ != nullptr) {
        player_controller_->Update(delta_time);
    }
    return 0; 
}

std::unique_ptr<Component> System::CreateFPSComponent(physics::Controller* controller, render::Camera* camera) {
    return std::move(std::make_unique<Component>(this, std::move(std::make_unique<FPSController>(controller, camera))));
}


};