#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "base/Window.h"
namespace render {

class Camera : public base::Component {
public: 
    // offset_ and forward_ is uses in view_ in System update 
    glm::vec3 offset_ = glm::vec3(2.0, -5.0, 0.0);
    glm::vec3 forward_ = glm::vec3(0.0, 0.0, -1.0);
    glm::vec3 up_ = glm::vec3(-1.0, 0.0, 0.0);
    // System updates view_ every frame base on entity transform
    glm::mat4 view_;
    glm::mat4 projection_;

    Camera (base::System* parent, base::Window* window) : base::Component(parent) { 
        projection_ = glm::perspective(glm::radians(45.f), window->GetRatio(), 0.1f, 1000.f);
    }
};
};