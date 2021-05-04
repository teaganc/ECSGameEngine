#include "System.h"
#include "base/Entity.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <tiny_obj_loader.h>

namespace render {

void System::RegisterComponent(Component* component) {
    components_.push_back(component);
}

void System::RegisterComponent(base::Component* component) {
    if (dynamic_cast<Camera*>(component)) {
        camera_ = static_cast<Camera*>(component);
    }
    else {
        RegisterComponent(static_cast<Component*>(component));
    }
}

void System::UnRegisterComponent(Component* component) {
    components_.erase(std::ranges::find(components_, component));
}

void System::UnRegisterComponent(base::Component* component){
    if (dynamic_cast<Camera*>(component)) {
        camera_ = nullptr;
    }
    else {
        UnRegisterComponent(static_cast<Component*>(component));
    }
}

int System::Start() {
    //glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    
    glClearColor(0.f, 0.f, 0.f, 1.0);

    return 0;
}
int System::End() {

    return 0;
}

int System::Update(double delta_time) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (camera_ != nullptr && camera_->entity_ != nullptr) {
        camera_->view_ = glm::translate(glm::lookAt(glm::vec3(0.0), camera_->forward_, camera_->up_), camera_->offset_);
        camera_->view_ *= camera_->entity_->GetTransform();
    }

    auto enabled = std::views::filter([](auto& i) { return i->enabled_;});
    auto draw = [this] (auto& component) { DrawComponent(component); };

    std::ranges::for_each(components_ | enabled, draw);

    window_->Swap();
    return 0;
}


Shader* System::GetShader(std::string shader_name) {
    if (!shaders_.contains(shader_name)) {
        shaders_.emplace(shader_name, std::make_unique<Shader>(shader_name + ".vert", shader_name + ".frag"));
    }
    return shaders_.find(shader_name)->second.get();
}

std::unique_ptr<Component> System::CreateFromGeometry(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::string shader_name) {
    return std::make_unique<Component>(this, Mesh(vertices, indices), Material(GetShader(shader_name)));
}

std::unique_ptr<Component> System::CreateFromFile(std::string object_file, std::string shader_name /*, TODO allocator */) {
    tinyobj::ObjReader reader;
    tinyobj::ObjReaderConfig reader_config;

    if (!reader.ParseFromFile(object_file, reader_config)) {
        if (!reader.Error().empty()) {
            assert(!("TinyObjLoader:" + reader.Error()).empty());
        }
    }

    auto& attrib = reader.GetAttrib();
    std::vector<Vertex> vertices;
    auto& shapes = reader.GetShapes();
    std::vector<uint32_t> indices;

    for (auto& shape : shapes) {
        /*
        size_t index_offset = 0;
        for (auto& fv : shape.mesh.num_face_vertices) {
            for (size_t v = 0; v < fv; ++v) {
                indices.push_back(vertices.size());

                tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
                vertices.push_back ( Vertex(
                    {attrib.vertices[3 * idx.vertex_index],
                    attrib.vertices[3 * idx.vertex_index + 1],
                    attrib.vertices[3 * idx.vertex_index + 2]},
                    {attrib.normals[3 * idx.normal_index],
                    attrib.normals[3 * idx.normal_index + 1],
                    attrib.normals[3 * idx.normal_index + 2]},
                    {attrib.normals[2 * idx.texcoord_index],
                    attrib.normals[2 * idx.texcoord_index + 1]}
                ));
            }
            index_offset += fv;
        }
        */
        int j = 0;
        for (auto& i : shape.mesh.indices) {
            indices.push_back(j++);
            vertices.push_back( Vertex(
                {attrib.vertices[3 * i.vertex_index + 0],
                attrib.vertices[3 * i.vertex_index + 1],
                attrib.vertices[3 * i.vertex_index + 2]},
                {attrib.normals[3 * i.normal_index + 0],
                attrib.normals[3 * i.normal_index + 1],
                attrib.normals[3 * i.normal_index + 2]},
                {attrib.texcoords[2 * i.texcoord_index + 0],
                attrib.texcoords[2 * i.texcoord_index + 1]}));
       }
    }

    Mesh mesh(vertices, indices);

    Material material(GetShader(shader_name));

    auto& materials = reader.GetMaterials();
    if (materials.size() > 0) {
        auto current = materials[0];

        std::vector<std::pair<const std::string&, Texture::Type>> material_textures = {
            {current.diffuse_texname, Texture::Type::DIFFUSE},
            {current.ambient_texname, Texture::Type::AMBIENT},
            {current.specular_texname, Texture::Type::SPECULAR},
            {current.normal_texname, Texture::Type::NORMAL}
        };

        for (auto& i: material_textures) {
            if (i.first.length() > 0) {
                material.textures_[i.second] = Texture::CreateFromFile(i.first, i.second);
                material.mask_ |= 1 << i.second;
            }
        } 
    }

    return std::make_unique<Component>(this, mesh, material);
}

Camera System::CreateCamera() {
    return Camera(this, window_);
}

void System::BindMaterial(Material* material) {
    if (material->shader_ != current_shader_) {
        //fprintf(stderr, "current shader: %i, next shader: %i\n", 0, material->shader_->ID);
        current_shader_ = material->shader_;
        current_shader_->Use();

        std::vector<std::pair<const std::string, Texture::Type>> texture_locations = {
            {"diffuse_texture_sampler", Texture::Type::DIFFUSE},
            {"ambient_texture_sampler", Texture::Type::AMBIENT},
            {"specular_texture_sampler", Texture::Type::SPECULAR},
            {"normal_texture_sampler", Texture::Type::NORMAL},
        };

        for (auto &i : texture_locations) {
            //fprintf(stderr, "Setting %s to %i\n", i.first.c_str(), i.second);
            current_shader_->SetInt(i.first, i.second);
        }
    }   

    for (auto& i : material->textures_) {
        glActiveTexture(GL_TEXTURE0 + (i.first) );
        glBindTexture(GL_TEXTURE_2D, i.second->texture_name_);
    }

    current_shader_->SetInt("mask", material->mask_);
}

void System::BindUniforms(Component* component) {
    glm::mat4 model = component->entity_->GetTransform();

    glm::mat4 view = glm::lookAt(glm::vec3(0.f, 2.f, -30.f),
            glm::vec3(0.f, 0.f, 0.f),
            glm::vec3(0.f, 1.f, 0.f));
    glm::mat4 projection = glm::perspective(glm::radians(45.f), 800.f / 600.f, 0.1f, 100.f);

    if (camera_ != nullptr) {
        //view = camera_->view_;
        //projection = camera_->projection_;
    }

    glm::mat4 MV = view * model;
    glm::mat4 MVP = projection * view * model;
    current_shader_->SetMat4("model", model);
    current_shader_->SetMat4("view", view);
    current_shader_->SetMat4("MV", MV);
    current_shader_->SetMat4("MVP", MVP);

    glm::vec3 light_position(0.f, 2.f,-20.f);
    current_shader_->SetVec3("light_position_ws", light_position);
}

void System::DrawComponent(Component* component) {
    BindMaterial(&component->material_);

    BindUniforms(component);

    component->mesh_.Draw(); // TODO bring mesh draw code into System
}

};