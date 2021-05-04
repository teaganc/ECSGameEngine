#pragma once

#include "Shader.h"
#include "Texture.h"

#include <memory>
#include <unordered_map>

namespace render {

class Material {
public:
    Shader* shader_;
    
    unsigned int mask_ = 0;
    std::unordered_map<Texture::Type, std::shared_ptr<Texture>> textures_;

    Material(Shader* shader) : shader_(shader) {}
    void Bind();

private:

};
};