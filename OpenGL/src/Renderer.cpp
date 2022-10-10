#include "Renderer.h"
#include "glad/glad.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"

Renderer::Renderer() {}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawArrays(const VertexArray& va, const Shader& shader, uint32_t num_of_vertices)
{
	shader.Bind();
	va.Bind();

	// param 2: starting index
	// param 3: num of vertices
	glDrawArrays(GL_TRIANGLES, 0, num_of_vertices);
}

void Renderer::DrawElements(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.Bind();
	va.Bind();
	ib.Bind();

	// param 2: num of indices
	// param 4: pointer to index buffer, but we can give nullptr since (if) we already have an index buffer bound)
	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawMesh(const Mesh& mesh, Shader &shader)
{
    uint32_t diffuse_texture_num{ 1 };
    uint32_t specular_texture_num{ 1 };
    uint32_t normal_texture_num{ 1 };
    uint32_t height_texture_num{ 1 };
    uint32_t roughness_texture_num{ 1 };
    const std::vector<Texture>* textures{ mesh.GetTextures() };

    for (size_t i{ 0 }; i < textures->size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        char texture_uniform_name[]{ "texture_XXXXXXXXXXX" };

        switch ((*textures)[i].GetType())
        {
        case Texture::Type::Diffuse:
            texture_uniform_name[15] = '0' + diffuse_texture_num++;
            texture_uniform_name[16] = '\0';
            break;

        case Texture::Type::Specular:
            texture_uniform_name[16] = '0' + specular_texture_num++;
            texture_uniform_name[17] = '\0';
            break;

        case Texture::Type::Normal:
            texture_uniform_name[14] = '0' + normal_texture_num++;
            texture_uniform_name[15] = '\0';
            break;

        case Texture::Type::Height:
            texture_uniform_name[14] = '0' + height_texture_num++;
            texture_uniform_name[15] = '\0';
            break;

        case Texture::Type::Roughness:
            texture_uniform_name[17] = '0' + roughness_texture_num++;
            texture_uniform_name[18] = '\0';
            break;

        default:
            break;
        }

        shader.SetUniform1i(texture_uniform_name, i);
        (*textures)[i].Bind();
    }

    mesh.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(mesh.GetIndices()->size()), GL_UNSIGNED_INT, 0);
    mesh.Unbind();
    glActiveTexture(GL_TEXTURE0);
}

void Renderer::DrawModel(const Model& model, Shader& shader)
{
    const std::vector<Mesh> *meshes{ model.GetMeshes() };

    for (size_t i{ 0 }; i < meshes->size(); ++i)
        DrawMesh((*meshes)[i], shader);
}
