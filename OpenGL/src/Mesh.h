#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
//#include "Vertex.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_set>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texture_coordinates;
    glm::vec3 tangent;
    glm::vec3 bitangent;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec2 texture_coordinates, glm::vec3 tangent, glm::vec3 bitangent)
    {
        this->position = position;
        this->normal = normal;
        this->texture_coordinates = texture_coordinates;
        this->tangent = tangent;
        this->bitangent = bitangent;
    }

    Vertex(const Vertex& vert)
    {
        this->position = vert.position;
        this->normal = vert.normal;
        this->texture_coordinates = vert.texture_coordinates;
        this->tangent = vert.tangent;
        this->bitangent = vert.bitangent;
    }
};

class Mesh {
public:
    Mesh(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices, std::vector<Texture>& textures);
    Mesh(const Mesh& mesh);
    Mesh(aiMesh* mesh, const aiScene* scene, std::unordered_set<std::string>& loaded_textures_file_names, const std::string& directory);

    void Bind() const;
    void Unbind() const;

    const std::vector<Vertex>* GetVertices() const;
    const std::vector<uint32_t>* GetIndices() const;
    const std::vector<Texture>* GetTextures() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

private:
    void Setup();
    void LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::unordered_set<std::string>& loaded_textures_file_names, Texture::Type texture_type, const std::string& directory);
};