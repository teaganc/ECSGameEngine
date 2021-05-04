#pragma once

#include "base/System.h"
#include "Component.h"
#include "base/Window.h"
#include "Camera.h"

#include <GL/gl.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace render {

class System : public base::System {
private:
    base::Window* window_;

    std::vector<Component*> components_;
    std::unordered_map<std::string, std::unique_ptr<Shader>> shaders_;

    Shader* current_shader_ = nullptr;

public:
    Camera* camera_ = nullptr;

    System(base::Window* window) : window_(window) {}

    void RegisterComponent(Component* component);
    void RegisterComponent(base::Component* component) override;
    void UnRegisterComponent(Component* component);
    void UnRegisterComponent(base::Component* component) override;

    int Start() override;
    int End() override;
    int Update(double delta_time) override;

    Shader* GetShader(std::string shader_name);

    std::unique_ptr<Component> CreateFromGeometry(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string shader_name);
    std::unique_ptr<Component> CreateFromFile(std::string object_file, std::string shader_name /*, TODO allocator */);

    Camera CreateCamera();
private: 

    void BindMaterial(Material* material);
    void BindUniforms(Component* component);
    void DrawComponent(Component* component);

};

};