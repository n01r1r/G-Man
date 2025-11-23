#pragma once

#include <vector>
#include <memory>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"

class Scene {
public:
    Scene();
    ~Scene();

    void Init();
    void Update(float deltaTime);
    void Render(float aspectRatio);
    void OnImGuiRender();
    void LoadModel(std::string path);
    void FocusCamera(Model* model);

    Camera camera;
    bool isCameraMouseActive = false;

private:
    std::unique_ptr<Shader> shader;
    std::vector<std::unique_ptr<Model>> models;
    
    // Scene Data
    DirLight dirLight;
    std::vector<PointLight> pointLights;
    std::vector<AreaLight> areaLights;

    // Material editing state
    int selectedModelIndex = -1;
    glm::vec3 materialColor = glm::vec3(1.0f);
    float materialShininess = 32.0f;

    // Model Transform
    glm::vec3 modelPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 modelRotation = glm::vec3(0.0f, 0.0f, 0.0f); // Euler angles in degrees
    glm::vec3 modelScale = glm::vec3(1.0f, 1.0f, 1.0f);

    void setupDefaultScene();
};
