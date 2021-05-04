#pragma once

#include <GL/gl.h>
#include <vector>

#include "Texture.h"
#include "Vertex.h"

namespace render {

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices) : vertices_(vertices), indices_(indices) {    
        Initialize();
    }

    std::vector<Vertex> vertices_;
    std::vector<uint32_t> indices_;

    void Draw();

private:
    GLuint vao_, vbo_, ebo_;

    void Initialize();
};

};