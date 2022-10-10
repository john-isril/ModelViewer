#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glad/glad.h>
#include <string>
#include <vector>
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"
#include "Texture.h"

class Mesh {
private:
    std::vector<Vertex> m_vertices;
    std::vector<uint32_t> m_indices;
    std::vector<Texture> m_textures;
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    IndexBuffer m_IBO;

private:
    void Setup();

public:
    Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32_t> &indices, const std::vector<Texture> &textures);
    Mesh(const Mesh& mesh);

    void Bind() const;
    void Unbind() const;

    const std::vector<Vertex>* GetVertices() const;
    const std::vector<uint32_t>* GetIndices() const;
    const std::vector<Texture>* GetTextures() const;

    void Draw(Shader& shader); // todo: this is the renderers responsibility
};