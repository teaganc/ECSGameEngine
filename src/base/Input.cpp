#include "Input.h"

#include <SDL2/SDL.h>

#include "Engine.h"

namespace base {

int Input::Poll() {
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
                break;
        }
    }
    return 0;
}

}