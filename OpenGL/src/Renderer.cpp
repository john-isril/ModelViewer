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
        char diffuse_texture_uniform_name[]{ "material.diffuse0" };
        char specular_texture_uniform_name[]{ "material.specular0" };
        char normal_texture_uniform_name[]{ "material.normal0" };
        char height_texture_uniform_name[]{ "material.height0" };
        char roughness_texture_uniform_name[]{ "material.roughness0" };

        switch ((*textures)[i].GetType())
        {
        case Texture::Type::Diffuse:
            diffuse_texture_uniform_name[16] = '0' + diffuse_texture_num++;
            shader.SetUniform1i(diffuse_texture_uniform_name, i);
            break;

        case Texture::Type::Specular:
            specular_texture_uniform_name[17] = '0' + specular_texture_num++;
            shader.SetUniform1i(specular_texture_uniform_name, i);
            break;

        case Texture::Type::Normal:
            normal_texture_uniform_name[15] = '0' + normal_texture_num++;
            shader.SetUniform1i(normal_texture_uniform_name, i);
            break;

        case Texture::Type::Height:
            height_texture_uniform_name[15] = '0' + height_texture_num++;
            shader.SetUniform1i(height_texture_uniform_name, i);
            break;

        case Texture::Type::Roughness:
            roughness_texture_uniform_name[18] = '0' + roughness_texture_num++;
            shader.SetUniform1i(roughness_texture_uniform_name, i);
            break;

        default:
            break;
        }

        (*textures)[i].Bind();
    }
    
    glActiveTexture(GL_TEXTURE0);

    mesh.Bind();
    glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(mesh.GetIndices()->size()), GL_UNSIGNED_INT, 0);
    mesh.Unbind();
}

void Renderer::DrawModel(const Model& model, Shader& shader)
{
    const std::vector<Mesh> *meshes{ model.GetMeshes() };

    for (size_t i{ 0 }; i < meshes->size(); ++i)
        DrawMesh((*meshes)[i], shader);
}

void Renderer::DrawSkybox(Skybox& skybox, const glm::mat4& view, const glm::mat4& projection, Shader& shader)
{
    glDepthFunc(GL_LEQUAL);
    shader.Bind();
    const glm::mat4 skybox_view = glm::mat4(glm::mat3(view));
    shader.SetUniformMat4f("view", skybox_view);
    shader.SetUniformMat4f("projection", projection);
    skybox.Activate();
    glDrawArrays(GL_TRIANGLES, 0, 36);
    skybox.Deactivate();
    glDepthFunc(GL_LESS);
}
