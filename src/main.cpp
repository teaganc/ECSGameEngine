#include "base/Engine.h"

#include "physics/System.h"
#include "render/Vertex.h"
#include "render/Component.h"
#include "render/System.h"

#include "render/Camera.h"
#include "input/FPSController.h"
#include "input/Component.h"
#include "input/System.h"

int main () {

    base::Engine engine;

    engine.AddSystem(std::make_unique<render::System>(engine.GetWindow()));
    engine.AddSystem(std::make_unique<physics::System>());
    engine.AddSystem(std::make_unique<input::System>(&engine));

    engine.Start(800, 600);

    auto physics_component = engine.GetSystem<physics::System>()->CreateFromNothing();
    auto render_component = engine.GetSystem<render::System>()->CreateFromFile("cube1.obj", "shader");

    auto graph = engine.GetGraph();

    base::Entity* entity = graph->AddNode(graph->GetHead(), std::make_unique<base::Entity>());
    entity->AddComponent(std::move(render_component));
    entity->AddComponent(std::move(physics_component));
    
    {
        std::vector<render::Vertex> vertices = {
            {{ 0.f, 19.f, 19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
            {{ 0.f,-19.f, 19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
            {{ 0.f,-19.f,-19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
            {{ 0.f,-19.f,-19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
            {{ 0.f, 19.f,-19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
            {{ 0.f, 19.f, 19.f}, {1.f,0.f,0.f}, {0.f, 0.f}},
        };

        std::vector<uint32_t> indices = {
            0,1,2 ,3,4,5
        };
        
        auto plane_component = engine.GetSystem<physics::System>()->CreateFromNothing2();
        auto plane_component2 = engine.GetSystem<render::System>()->CreateFromGeometry(vertices, indices, "shader");

        entity = graph->AddNode(graph->GetHead(), std::make_unique<base::Entity>());
        entity->AddComponent(std::move(plane_component));
        entity->AddComponent(std::move(plane_component2));
    }
    {
        auto physics_component3 = engine.GetSystem<physics::System>()->CreateFromNothing(physx::PxVec3(0.5, 5, 0.5));
        auto render_component3 = engine.GetSystem<render::System>()->CreateFromFile("monkey1.obj", "shader");

        entity = graph->AddNode(graph->GetHead(), std::make_unique<base::Entity>());
        entity->AddComponent(std::move(render_component3));
        entity->AddComponent(std::move(physics_component3));
    }
    {
        auto camera_component = std::make_unique<render::Camera>(engine.GetSystem<render::System>(), engine.GetWindow());
        auto controller_component = engine.GetSystem<physics::System>()->CreateController();
        auto FPSController = engine.GetSystem<input::System>()->CreateFPSComponent(controller_component.get(), camera_component.get());

        entity = graph->AddNode(graph->GetHead(), std::make_unique<base::Entity>());
        entity->AddComponent(std::move(camera_component));
        entity->AddComponent(std::move(controller_component));
        //entity4.AddComponent(std::move(FPSController));
    }
    engine.Execute();

    engine.End();

    return 0;
}