#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <PxPhysics.h>
#include <vector>

#include "Component.h"
namespace base {

class Entity {
public:
    // read only. These get updated by the physics system. should probably have accesors instead
    glm::vec3 position_;
    glm::quat rotation_;

    void AddComponent(std::unique_ptr<Component> component);
    void RemoveComponent(Component* component);

    glm::mat4 GetTransform();
    void SetTransform(const physx::PxTransform& transform);

    template <std::derived_from<Component> T> 
    T* GetComponent() {
        auto item = std::ranges::find_if(components_, [] (auto& i) { 
            return dynamic_cast<T*>(i.get()) != nullptr; });
        if (item != components_.end()) {
            return static_cast<T*>(item->get());
        }
        return nullptr;
    }

private:
    std::vector<std::unique_ptr<Component>> components_;
};

};