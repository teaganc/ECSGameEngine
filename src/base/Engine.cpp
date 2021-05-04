#include "Engine.h"

#include <algorithm>
namespace base {

Engine::Engine() : window_ (std::make_unique<Window>()), graph_(std::make_unique<Graph>()) {

}

int Engine::Start(int width, int height) {
    window_.get()->Start(width, height);

    auto start = [](auto& i) { i.get()->Start(); };
    std::ranges::for_each(systems_, start);

    is_running = true;
    return 0;
}

int Engine::End(){
    window_.get()->End();
    return 0;
}

int Engine::Execute(){
    
    while (is_running) {        
        uint32_t current_tick = SDL_GetTicks();
        double delta_time = (current_tick - last_tick) / 1000.0;

        auto update = [delta_time](auto& i) { i.get()->Update(delta_time); };

        std::ranges::for_each(systems_, update);
    
        last_tick = current_tick;
    }
    return 0;
}

void Engine::AddSystem(std::unique_ptr<System> system) {
    systems_.push_back(std::move(system));
}

};