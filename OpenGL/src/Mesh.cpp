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

void Mesh::Draw(Shader& shader)
{
    // bind appropriate textures
    uint32_t diffuse_texture_num{ 1 };
    uint32_t specular_texture_num{ 1 };
    uint32_t normal_texture_num{ 1 };
    uint32_t height_texture_num{ 1 };
    uint32_t roughness_texture_num{ 1 };

    for (size_t i{ 0 }; i < m_textures.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        char texture_uniform_name[]{"texture_XXXXXXXXXXX"};

        switch (m_textures[i].GetType())
        {
        case Texture::Type::Diffuse:
            texture_uniform_name[15] = '0' + diffuse_texture_num++;
            texture_uniform_name[16] = '\0';
            //texture_uniform_name = "texture_diffuse" + std::to_string(diffuse_texture_num++);
            break;

        case Texture::Type::Specular:
            texture_uniform_name[16] = '0' + specular_texture_num++;
            texture_uniform_name[17] = '\0';
            //texture_uniform_name = "texture_specular" + std::to_string(specular_texture_num++);
            break;

        case Texture::Type::Normal:
            texture_uniform_name[14] = '0' + normal_texture_num++;
            texture_uniform_name[15] = '\0';
            //texture_uniform_name = "texture_normal" + std::to_string(normal_texture_num++);
            break;

        case Texture::Type::Height:
            texture_uniform_name[14] = '0' + height_texture_num++;
            texture_uniform_name[15] = '\0';
            //texture_uniform_name = "texture_height" + std::to_string(height_texture_num++);
            break;

        case Texture::Type::Roughness:
            texture_uniform_name[17] = '0' + roughness_texture_num++;
            texture_uniform_name[18] = '\0';
            //texture_uniform_name = "texture_roughness" + std::to_string(roughness_texture_num++);
            break;

        default:
            break;
        }

        shader.SetUniform1i(texture_uniform_name, i);
        m_textures[i].Bind();
    }

    // draw mesh
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_indices.size()), GL_UNSIGNED_INT, 0);
    m_VAO.Unbind();

    glActiveTexture(GL_TEXTURE0);
}
