#pragma once

#include "base/Component.h"
#include "Material.h"
#include "Mesh.h"

namespace render {

class Component : public base::Component {
public:
    Mesh mesh_;
    Material material_;    
    bool enabled_ = true;

    Component(base::System* parent, Mesh mesh, Material material) : base::Component(parent), mesh_(mesh), material_(material) {
    };

};

};