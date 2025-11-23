#pragma once

#include <glm/glm.hpp>

struct DirLight {
    glm::vec3 direction;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct PointLight {
    glm::vec3 position;
    
    float constant;
    float linear;
    float quadratic;
    
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
};

struct AreaLight {
    glm::vec3 position;
    glm::vec3 points[4]; // For a rectangular area light
    glm::vec3 color;
    float intensity;
    
    // For simplicity in this iteration, we might just treat it as a glowing quad or approximate it
    // But we'll store the data here.
};
