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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures) :
    m_vertices{ vertices }, m_indices{ indices }, m_textures{ textures },
    m_VAO{}, m_VBO{ &vertices[0], vertices.size() * sizeof(Vertex) }, m_IBO{ &indices[0], m_indices.size() }
{

    // now that we have all the required data, set the vertex buffers and its attribute pointers.
    Setup();
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
        
        std::string texture_uniform_name;

        switch (m_textures[i].GetType())
        {
        case Texture::Type::Diffuse:
            texture_uniform_name = "texture_diffuse" + std::to_string(diffuse_texture_num++);
            break;

        case Texture::Type::Specular:
            texture_uniform_name = "texture_specular" + std::to_string(specular_texture_num++);
            break;

        case Texture::Type::Normal:
            texture_uniform_name = "texture_normal" + std::to_string(normal_texture_num++);
            break;

        case Texture::Type::Height:
            texture_uniform_name = "texture_height" + std::to_string(height_texture_num++);
            break;

        case Texture::Type::Roughness:
            texture_uniform_name = "texture_roughness" + std::to_string(roughness_texture_num++);
            break;

        default:
            break;
        }

        shader.SetUniform1i(texture_uniform_name.c_str(), i);
        m_textures[i].Bind();
    }

    // draw mesh
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_indices.size()), GL_UNSIGNED_INT, 0);
    m_VAO.Unbind();

    glActiveTexture(GL_TEXTURE0);
}
