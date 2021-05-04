#pragma once
#include <glm/glm.hpp>

namespace render {

class Vertex {
public:
    glm::vec3 position_;
    glm::vec3 normal_;
    glm::vec2 tex_coord_;
};

};