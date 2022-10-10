#pragma once
#include <glm/glm.hpp>

#define MAX_BONE_INFLUENCE 4

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    uint32_t m_BoneIDs[MAX_BONE_INFLUENCE];
    float m_Weights[MAX_BONE_INFLUENCE];

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec3 tangent, glm::vec3 bitangent)
    {
        this->position = position;
        this->normal = normal;
        this->texture_coordinates = texture_coordinates;
        this->tangent = tangent;
        this->bitangent = bitangent;
    }
};