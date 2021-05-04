#pragma once

#include <GL/glew.h>
#include<GL/gl.h>
#include <memory>
#include <string>
#include <unordered_map>

namespace render {

class Texture {
public:
    enum Type {
        DIFFUSE,
        AMBIENT,
        SPECULAR,
        NORMAL,
    };

    GLuint texture_name_;
    Type type_;

    Texture(GLuint texture_name, Type type) : texture_name_ (texture_name), type_ (type){}

    static std::shared_ptr<Texture> CreateFromFile(const std::string filename, Type type);
    static std::unordered_map<std::string, std::shared_ptr<Texture>> textures_;

};

};