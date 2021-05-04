#include "FPSController.h"

#include <glm/gtx/rotate_vector.hpp>

namespace input {

FPSController::FPSController(physics::Controller* capsule_controller, render::Camera* camera) 
: capsule_controller_(capsule_controller), camera_(camera) {
    button_binds_ = {
        {WALK_FORWARD, [this](void) { this->WalkForward(); }},
        {WALK_BACK, [this](void) { this->WalkBack(); }},
        {WALK_RIGHT, [this](void) { this->WalkRight(); }},
        {WALK_LEFT, [this](void) { this->WalkLeft(); }}
    };
}

void FPSController::Register(base::Entity* entity) {
}


void FPSController::WalkForward() {
    delta_walk_[0] += 1.f;
}

void FPSController::WalkBack() {
    delta_walk_[0] -= 1.f;
}
void FPSController::WalkRight() {
    delta_walk_[1] += 1.f;
}
void FPSController::WalkLeft() {
    delta_walk_[1] -= 1.f;
}

int FPSController::Update(float delta_time) {

    camera_->forward_ = glm::normalize(glm::rotate(camera_->forward_, delta_look_[0], camera_->up_));
    glm::vec3 right = glm::normalize(glm::cross(camera_->forward_, camera_->up_)); 
    camera_->forward_ = glm::rotate(camera_->forward_, delta_look_[1], right);

    glm::vec3 move = glm::normalize(camera_->forward_);
    move.y = 0.0f;
    move = move * delta_walk_[0] + right * delta_walk_[1];
    physx::PxVec3 movement = physx::PxVec3(move.x, move.y, move.z);
    physx::PxControllerFilters filters;

    capsule_controller_->capsule_controller_->move(movement, 0.001, delta_time, filters);

    base::Component* component = static_cast<base::Component*>(capsule_controller_->capsule_controller_->getActor()->userData);
    if (component->entity_ != nullptr) {
        component->entity_->SetTransform(capsule_controller_->capsule_controller_->getActor()->getGlobalPose());
    }

    if (movement.z != 0.0) {
        fprintf(stderr, "%f, %f, %f, %f\n", delta_time, movement.x, movement.y, movement.z);
        fprintf(stderr, "%f\n", capsule_controller_->entity_->position_.y);
        fprintf(stderr, "%f\n", capsule_controller_->capsule_controller_->getPosition().y);
    }



    delta_walk_[0] = 0.f;
    delta_walk_[1] = 0.f;
    delta_look_[0] = 0.f;
    delta_look_[1] = 0.f;

    return 0;
}

};