#pragma once

#include "Component.h"

namespace physics {

class Controller : public base::Component {
public:
    // this is an owning pointer 
    physx::PxCapsuleController* capsule_controller_;

    Controller (base::System* parent, physx::PxCapsuleController* capsule) : base::Component(parent) , capsule_controller_ (capsule) {
        capsule_controller_->getActor()->userData = this;
    }
    ~Controller() {
        capsule_controller_->release();
    }

};

};