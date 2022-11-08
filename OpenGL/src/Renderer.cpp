#include "Renderer.h"
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "Model.h"
#include "DebugUtils.h"

Renderer::Renderer() 
{
    Renderer::Init();
}

void Renderer::Init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwTerminate();
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CW);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); // if they pass (both stencil and depth tests) replace them with 1s
    glDebugMessageCallback(DebugUtils::OpenGLMessageCallback, nullptr);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Clear(const glm::vec3& color, uint64_t mask)
{
	glClearColor(color.r, color.g, color.b, 1.0f);
	glClear(mask);
}

void Renderer::EnableStencilTest()
{
    glEnable(GL_STENCIL_TEST);
}

void Renderer::SetStencilFunc(GLenum func, GLint ref, GLuint mask)
{
    glStencilFunc(func, ref, mask);
}

void Renderer::SetStencilMask(GLuint mask)
{
    glStencilMask(mask);
}

void Renderer::DisableStencilTest()
{
    glDisable(GL_STENCIL_TEST);
}

void Renderer::EnableDepthTest()
{
    glEnable(GL_DEPTH_TEST);
}

void Renderer::DisableDepthTest()
{
    glDisable(GL_DEPTH_TEST);
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
    uint32_t alpha_texture_num{ 1 };
    const std::vector<Texture>* textures{ mesh.GetTextures() };

    for (size_t i{ 0 }; i < textures->size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        char diffuse_texture_uniform_name[]{ "material.diffuse0" };
        char specular_texture_uniform_name[]{ "material.specular0" };
        char normal_texture_uniform_name[]{ "material.normal0" };
        char height_texture_uniform_name[]{ "material.height0" };
        char roughness_texture_uniform_name[]{ "material.roughness0" };
        char alpha_texture_uniform_name[]{ "material.alpha0" };

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
            //shader.SetUniform1i(normal_texture_uniform_name, i);
            break;

        case Texture::Type::Height:
            height_texture_uniform_name[15] = '0' + height_texture_num++;
            shader.SetUniform1i(height_texture_uniform_name, i);
            break;

        case Texture::Type::Roughness:
            roughness_texture_uniform_name[18] = '0' + roughness_texture_num++;
            shader.SetUniform1i(roughness_texture_uniform_name, i);
            break;

        case Texture::Type::Alpha:
            alpha_texture_uniform_name[14] = '0' + alpha_texture_num++;
            shader.SetUniform1i(alpha_texture_uniform_name, i);
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

void Renderer::DrawModelOutline(Model& model, const glm::mat4& view, const glm::mat4& projection, Shader& outline_shader, float outline_scale, const glm::vec3& outline_color)
{
    SetStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    SetStencilMask(0x00);
    //DisableDepthTest();

    outline_shader.Bind();
    outline_shader.SetUniformMat4f("mvp", model.GetTransform().GetScaledCopyMVP(1.05, view, projection));
    outline_shader.SetUniformVec3f("outline_color", outline_color);
    outline_shader.SetUniform1f("outline_scale", outline_scale);
    DrawModel(model, outline_shader);
}

void Renderer::EnableOutlining()
{
    SetStencilFunc(GL_ALWAYS, 1, 0xFF);
    SetStencilMask(0xFF);
}

void Renderer::DisableOutlining()
{
    SetStencilMask(0xFF);
    SetStencilFunc(GL_ALWAYS, 1, 0xFF);
    EnableDepthTest();
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
