#include "Window.h"

#include <GL/glew.h>

namespace base {

int Window::Start(int width, int height) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    window_ = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    context_ = SDL_GL_CreateContext(window_);

    glewExperimental = GL_TRUE;

    glewInit();

    return 0;
}

int Window::End() {
    SDL_GL_DeleteContext(context_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    
    return 0;
}

void Window::Swap() {
    SDL_GL_SwapWindow(window_);
}


int Window::GetWidth() {
    int x, y;
    SDL_GetWindowSize(window_, &x, &y);
    return x;
}

int Window::GetHeight(){
    int x, y;
    SDL_GetWindowSize(window_, &x, &y);
    return y;
}

float Window::GetRatio() {
    int x, y;
    SDL_GetWindowSize(window_, &x, &y);
    return float(x) / float(y);
}

};