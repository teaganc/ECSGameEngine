#include "Material.h"

namespace render {

void Material::Bind() {
    shader_->Use();
}
};