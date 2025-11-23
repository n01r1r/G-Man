#include "Scene.h"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <glm/gtc/type_ptr.hpp>

Scene::Scene() : camera(glm::vec3(0.0f, 1.0f, 5.0f)) {
    modelScale = glm::vec3(0.01f); // Default scale down for large models
}

Scene::~Scene() {
}

void Scene::Init() {
    // Load Shader
    shader = std::make_unique<Shader>("assets/shaders/default.vert", "assets/shaders/default.frag");

    // Setup Lights
    dirLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    dirLight.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    dirLight.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    dirLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    PointLight pLight;
    pLight.position = glm::vec3(0.7f, 0.2f, 2.0f);
    pLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    pLight.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    pLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pLight.constant = 1.0f;
    pLight.linear = 0.09f;
    pLight.quadratic = 0.032f;
    pointLights.push_back(pLight);

    // Load Default Model
    try {
        // models.push_back(std::make_unique<Model>("assets/models/backpack/backpack.obj"));
        spdlog::info("Scene Initialized.");
    } catch (...) {
        spdlog::error("Failed to load default model.");
    }
}

void Scene::Update(float deltaTime) {
    // Camera input processing is handled in main loop via callbacks or polling, 
    // but we can put per-frame logic here.
}

void Scene::Render(float aspectRatio) {
    if (!shader) return;

    shader->use();

    // View/Projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), aspectRatio, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("projection", projection);
    shader->setMat4("view", view);

    // Set Light Uniforms
    shader->setVec3("dirLight.direction", dirLight.direction);
    shader->setVec3("dirLight.ambient", dirLight.ambient);
    shader->setVec3("dirLight.diffuse", dirLight.diffuse);
    shader->setVec3("dirLight.specular", dirLight.specular);

    for (size_t i = 0; i < pointLights.size(); i++) {
        std::string number = std::to_string(i);
        shader->setVec3("pointLights[" + number + "].position", pointLights[i].position);
        shader->setVec3("pointLights[" + number + "].ambient", pointLights[i].ambient);
        shader->setVec3("pointLights[" + number + "].diffuse", pointLights[i].diffuse);
        shader->setVec3("pointLights[" + number + "].specular", pointLights[i].specular);
        shader->setFloat("pointLights[" + number + "].constant", pointLights[i].constant);
        shader->setFloat("pointLights[" + number + "].linear", pointLights[i].linear);
        shader->setFloat("pointLights[" + number + "].quadratic", pointLights[i].quadratic);
    }
    // Set point light count (if shader uses loop with count, or just fixed size)
    // For this simple shader, we'll assume a fixed max number or set a uniform for count.
    shader->setInt("nrPointLights", (int)pointLights.size());

    // Set View Position
    shader->setVec3("viewPos", camera.Position);

    // Material Properties (Global override for now, or per model)
    shader->setVec3("material.color", materialColor);
    shader->setFloat("material.shininess", materialShininess);

    // Render Models
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, modelPosition);
    model = glm::rotate(model, glm::radians(modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, modelScale);
    shader->setMat4("model", model);

    for (auto& m : models) {
        m->Draw(*shader);
    }
}

void Scene::OnImGuiRender() {
    ImGui::Begin("Scene Editor");

    if (ImGui::CollapsingHeader("Camera")) {
        ImGui::DragFloat3("Position", (float*)&camera.Position, 0.1f);
        ImGui::DragFloat("Speed", &camera.MovementSpeed, 0.1f);
        ImGui::DragFloat("Zoom", &camera.Zoom, 0.1f, 1.0f, 45.0f);
    }

    if (ImGui::CollapsingHeader("Lights")) {
        ImGui::Text("Directional Light");
        ImGui::DragFloat3("Dir Direction", (float*)&dirLight.direction, 0.1f);
        ImGui::ColorEdit3("Dir Ambient", (float*)&dirLight.ambient);
        ImGui::ColorEdit3("Dir Diffuse", (float*)&dirLight.diffuse);
        
        ImGui::Separator();
        ImGui::Text("Point Lights");
        for (size_t i = 0; i < pointLights.size(); i++) {
            std::string label = "Point Light " + std::to_string(i);
            if (ImGui::TreeNode(label.c_str())) {
                ImGui::DragFloat3("Position", (float*)&pointLights[i].position, 0.1f);
                ImGui::ColorEdit3("Diffuse", (float*)&pointLights[i].diffuse);
                ImGui::DragFloat("Linear", &pointLights[i].linear, 0.01f);
                ImGui::DragFloat("Quadratic", &pointLights[i].quadratic, 0.01f);
                ImGui::TreePop();
            }
        }
        if (ImGui::Button("Add Point Light")) {
            PointLight pl;
            pl.position = camera.Position + camera.Front * 2.0f;
            pl.ambient = glm::vec3(0.05f);
            pl.diffuse = glm::vec3(0.8f);
            pl.specular = glm::vec3(1.0f);
            pl.constant = 1.0f;
            pl.linear = 0.09f;
            pl.quadratic = 0.032f;
            pointLights.push_back(pl);
        }
    }

    if (ImGui::CollapsingHeader("Material / Models")) {
        ImGui::Text("Transform");
        ImGui::DragFloat3("Position", (float*)&modelPosition, 0.1f);
        ImGui::DragFloat3("Rotation", (float*)&modelRotation, 1.0f);
        if (ImGui::Button("Fix Z-Up (-90 X)")) {
            modelRotation.x = -90.0f;
        }
        ImGui::SameLine();
        if (ImGui::Button("Reset Rot")) {
            modelRotation = glm::vec3(0.0f);
        }
        ImGui::DragFloat3("Scale", (float*)&modelScale, 0.001f);

        ImGui::Separator();
        ImGui::Text("Material");
        ImGui::ColorEdit3("Base Color", (float*)&materialColor);
        ImGui::DragFloat("Shininess", &materialShininess, 1.0f, 1.0f, 256.0f);
        
        ImGui::Separator();
        static char modelPath[128] = "assets/models/g-man-blacksuit/extracted/scene.gltf";
        ImGui::InputText("Model Path", modelPath, IM_ARRAYSIZE(modelPath));
        if (ImGui::Button("Load Model")) {
            try {
                models.push_back(std::make_unique<Model>(modelPath));
                spdlog::info("Loaded model: {}", modelPath);
            } catch (std::exception& e) {
                spdlog::error("Failed to load model: {}", e.what());
            }
        }
        
        if (ImGui::Button("Clear Models")) {
            models.clear();
        }
    }

    ImGui::End();
}

void Scene::LoadModel(std::string path) {
    try {
        auto model = std::make_unique<Model>(path);
        FocusCamera(model.get());
        models.push_back(std::move(model));
        spdlog::info("Loaded model: {}", path);
    } catch (std::exception& e) {
        spdlog::error("Failed to load model: {}", e.what());
    }
}

void Scene::FocusCamera(Model* model) {
    // Calculate center and size
    glm::vec3 center = (model->minBound + model->maxBound) / 2.0f;
    glm::vec3 size = model->maxBound - model->minBound;
    float maxDim = std::max(std::max(size.x, size.y), size.z);

    // Reset transform
    modelPosition = -center; // Center the model at origin
    modelRotation = glm::vec3(0.0f);
    
    // Scale to fit in a unit cube (roughly)
    float scaleFactor = 2.0f / maxDim;
    modelScale = glm::vec3(scaleFactor);

    // Position camera to view the model
    // Assuming model is now roughly unit size at origin
    camera.Position = glm::vec3(0.0f, 0.0f, 3.0f); 
    camera.Yaw = -90.0f;
    camera.Pitch = 0.0f;
    camera.updateCameraVectors();
}
