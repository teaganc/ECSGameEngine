#include "Entity.h"
#include "System.h"

#include <algorithm>

namespace base {

void Entity::AddComponent(std::unique_ptr<Component> component) {
    component->entity_ = this;
    component->parent_->RegisterComponent(component.get());
    components_.push_back(std::move(component));
}

void Entity::RemoveComponent(Component* component) {
    component->entity_ = nullptr;

    auto result = std::ranges::find_if(components_, 
    [component](auto& i) { return i.get() == component;});

    if (result != components_.end()) {
        result->get()->parent_->UnRegisterComponent(result->get());
        components_.erase(result);
    } 
}

glm::mat4 Entity::GetTransform() {
    // TODO fix this
    glm::mat4 transform(1.0);
    transform = glm::translate(transform, position_);
    glm::mat4 rotate = glm::toMat4(rotation_);
    return transform * rotate;
}

void Entity::SetTransform(const physx::PxTransform& transform) {
    position_.x = transform.p.x;
    position_.y = transform.p.y;
    position_.z = transform.p.z;            
    rotation_.x = transform.q.x;
    rotation_.y = transform.q.y;
    rotation_.z = transform.q.z;
    rotation_.w = transform.q.w;
}

};