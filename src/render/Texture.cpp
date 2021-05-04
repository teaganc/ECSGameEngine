#include "Texture.h"

#include "stb_image.h"

namespace render {
 std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::textures_;

std::shared_ptr<Texture> Texture::CreateFromFile(const std::string filename, Type type) {
    auto iterator = textures_.find(filename);
    if (iterator != textures_.end()) {
        return iterator->second;
    }

    unsigned int texture_name;
    unsigned char *image;
    int w, h, n, intfmt = 0, fmt = 0;

    image = stbi_load(filename.c_str(), &w, &h, &n, 0);
    if (!image) {
        fprintf(stderr, "cannot load texture '%s'\n", filename.c_str());
        return 0;
    }

    switch (n) {
        case 1: intfmt = fmt = GL_LUMINANCE; break;
        case 2: intfmt = fmt = GL_LUMINANCE_ALPHA; break;
        case 3: intfmt = fmt = GL_RGB; break;
        case 4: intfmt = fmt = GL_RGBA; break;
    }

    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    //glGenerateMipmap(GL_TEXTURE_2D);

    glTexImage2D(GL_TEXTURE_2D, 0, intfmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, image);

    stbi_image_free(image);

    auto ret = std::make_shared<Texture>(texture_name, type);

    textures_[filename] = ret;

    return ret;
}

};
