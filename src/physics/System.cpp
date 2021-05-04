#include "System.h"
#include "base/Entity.h"

namespace physics {

void System::RegisterComponent(Component* component){
    scene_->addActor(*component->body_);

    if (component->entity_ != nullptr) {
        component->entity_->SetTransform(component->body_->getGlobalPose());
    }
}
void System::RegisterComponent(base::Component* component) {
    if (dynamic_cast<Controller*>(component) != nullptr) {
        if (component->entity_ != nullptr) {
            component->entity_->SetTransform(static_cast<Controller*>(component)->capsule_controller_->getActor()->getGlobalPose());
            return;
        }
    }
    RegisterComponent(static_cast<Component*>(component));
}

void System::UnRegisterComponent(Component* component){
    scene_->removeActor(*component->body_);
}

void System::UnRegisterComponent(base::Component* component){
    if (dynamic_cast<Controller*>(component) != nullptr) {
        return;
    }
    UnRegisterComponent(static_cast<Component*>(component));
}

int System::Start() {
    foundation_ = PxCreateFoundation(PX_PHYSICS_VERSION, allocator_, error_callback_);
    physics_ = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation_, physx::PxTolerancesScale(), true);

    physx::PxSceneDesc scene_desc(physics_->getTolerancesScale());
    scene_desc.gravity = physx::PxVec3(0.f, -9.81f, 0.f);
    dispatcher_ = physx::PxDefaultCpuDispatcherCreate(2);
    scene_desc.cpuDispatcher = dispatcher_;
    scene_desc.filterShader = physx::PxDefaultSimulationFilterShader;
    scene_desc.flags.set(physx::PxSceneFlag::eENABLE_ACTIVE_ACTORS);
    //scene_desc.flags.set(physx::PxSceneFlag::eEXCLUDE_KINEMATICS_FROM_ACTIVE_ACTORS);

    scene_ = physics_->createScene(scene_desc);

    controller_manager_ = PxCreateControllerManager(*scene_);

    return 0;
}

int System::End() {
    scene_->release(); 
    dispatcher_->release(); 
    physics_->release();
    foundation_->release();

    return 0;
}

int System::Update(double delta_time){
    scene_->simulate(delta_time);
    scene_->fetchResults(true);

    physx::PxU32 num_active_actors;
    physx::PxActor** active_actors = scene_->getActiveActors(num_active_actors);

    for (physx::PxU32 i = 0; i < num_active_actors; ++i) {
        Component* component = static_cast<Component*>(active_actors[i]->userData);
        if (component->entity_ != nullptr) {
            component->entity_->SetTransform(component->body_->getGlobalPose());
        }
    }

    return 0;
}

std::unique_ptr<Component> System::CreateFromNothing(physx::PxVec3 pos) {
    physx::PxReal density = 1.0f;
    physx::PxTransform transform (pos);
    physx::PxVec3 dimensions(1.0,1.0,1.0);
    physx::PxMaterial* material = physics_->createMaterial(0.5, 0.5, 0.5);
    physx::PxShape* shape = physics_->createShape(physx::PxBoxGeometry(dimensions), *material);

    physx::PxRigidDynamic* body = physics_->createRigidDynamic(transform);
    
    body->attachShape(*shape);
    body->setAngularDamping(0.75);
    body->setLinearVelocity(physx::PxVec3(0,1,0));
    body->setMass(density);

    shape->release();

    return std::make_unique<Component>(this, body);
}

std::unique_ptr<Component> System::CreateFromNothing2() {
    physx::PxTransform transform (physx::PxVec3(0.f, -5.f, 0.f), physx::PxQuat(physx::PxPi / 2.0, physx::PxVec3(0.f, 0.f, 1.f)));
	physx::PxMaterial* material = physics_->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* shape = physics_->createShape(physx::PxPlaneGeometry(), *material);
	physx::PxRigidStatic* body = physics_->createRigidStatic(transform);

    body->attachShape(*shape);

    shape->release();

    return std::make_unique<Component>(this, body);
}


std::unique_ptr<Controller> System::CreateController(physx::PxCapsuleControllerDesc desc) {
    if (!desc.isValid()) {
        desc.height = 4;
        desc.radius = 2;
        desc.position = physx::PxExtendedVec3(0,3,-20);
        desc.material = physics_->createMaterial(0.5f, 0.5f, 0.3f);
        desc.density = 100.f;
        desc.contactOffset = 0.05f;
        desc.slopeLimit = 0.2f;
        desc.stepOffset = 0.75f;
    }
    return std::move(std::make_unique<Controller>(this, static_cast<physx::PxCapsuleController*> (controller_manager_->createController(desc))));
}


};