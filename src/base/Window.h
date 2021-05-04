#pragma once

#include <SDL2/SDL.h>

namespace base {

class Window {
private:
    SDL_Window* window_;
    SDL_GLContext context_;

public:
    
    int Start(int width, int height);
    int End();
    
    void Swap();

    int GetWidth();
    int GetHeight();
    float GetRatio();
};

};
