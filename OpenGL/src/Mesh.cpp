#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"
#include <string>
void Mesh::Setup()
{
	m_VAO.AddBufferLayoutMeshVertex(m_VBO);
	m_VAO.Unbind();
}

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const std::vector<Texture>& textures) :
    m_vertices{ vertices }, m_indices{ indices }, m_textures{ textures },
    m_VAO{}, m_VBO{ &vertices[0], vertices.size() * sizeof(Vertex) }, m_IBO{ &indices[0], m_indices.size() }
{

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    Setup();
}

Mesh::Mesh(const Mesh& mesh) :
    Mesh(mesh.m_vertices, mesh.m_indices, mesh.m_textures)
{
    
}

void Mesh::Bind() const
{
    m_VAO.Bind();
}

void Mesh::Unbind() const
{
    m_VAO.Unbind();
}

const std::vector<Vertex>* Mesh::GetVertices() const
{
    return &m_vertices;
}

const std::vector<uint32_t>* Mesh::GetIndices() const
{
    return &m_indices;
}

const std::vector<Texture>* Mesh::GetTextures() const
{
    return &m_textures;
}
