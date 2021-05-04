#pragma once 

#include "base/System.h"
#include "Component.h"
#include "Controller.h"

#include <memory>
#include <PxPhysicsAPI.h>

namespace physics {

class System : public base::System {
public:
    void RegisterComponent(Component* component);
    void RegisterComponent(base::Component* component) override;
    void UnRegisterComponent(Component* component);
    void UnRegisterComponent(base::Component* component) override;

    int Start() override;
    int End() override;
    int Update(double delta_time) override;

    std::unique_ptr<Component> CreateFromNothing(physx::PxVec3 pos = physx::PxVec3(0.f, 10.f, 0.f));
    std::unique_ptr<Component> CreateFromNothing2();

    std::unique_ptr<Controller> CreateController(physx::PxCapsuleControllerDesc desc = physx::PxCapsuleControllerDesc());

    
private:
    physx::PxFoundation* foundation_ = nullptr;
    physx::PxPhysics* physics_ = nullptr;
    physx::PxPvd* pvd_ = nullptr;
    physx::PxDefaultCpuDispatcher* dispatcher_ = nullptr;
    physx::PxScene* scene_ = nullptr;
    physx::PxControllerManager* controller_manager_ = nullptr;

    physx::PxDefaultAllocator allocator_;
    physx::PxDefaultErrorCallback error_callback_;

};

};