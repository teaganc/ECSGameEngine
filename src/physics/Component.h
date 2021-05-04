#pragma once 

#include "base/Component.h"

#include <PxPhysicsAPI.h>


namespace physics {

class Component : public base::Component {
public: 
    Component (base::System* parent, physx::PxRigidActor* body) : base::Component(parent), body_ (body) {
        body_->userData = this;
    }
    ~Component() {
        body_->release();
    }
    /* this is an owning pointer so it really needs 
    a unique_ptr dervivative that constructs and 
    deletes it. For now its done manually
    */
    physx::PxRigidActor* body_;
};

};