#pragma once
#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Skybox.h"

class Renderer
{
private:
	static Renderer m_instance;

public:
	Renderer();
	Renderer(const Renderer&) = delete;
	Renderer& operator=(const Renderer&) = delete;
	Renderer(Renderer&&) = delete;
	Renderer& operator=(Renderer&&) = delete;

	static void Init();
	static void Clear();
	static void Clear(const glm::vec3 &color, uint64_t mask);
	static void EnableStencilTest();
	static void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
	static void SetStencilMask(GLuint mask);
	static void DisableStencilTest();
	static void EnableDepthTest();
	static void DisableDepthTest();
	static void DrawArrays(const class VertexArray& va, const class Shader& shader, uint32_t num_of_vertices);
	static void DrawElements(const class VertexArray& va, const class IndexBuffer& ib, const class Shader& shader);
	static void DrawMesh(const class Mesh& mesh, Shader& shader);
	static void DrawModel(const class Model& model, Shader& shader);
	static void DrawModelOutline(Model& model, const glm::mat4& view, const glm::mat4& projection, Shader& outline_shader, float outline_scale, const glm::vec3 &outline_color);
	static void EnableOutlining();
	static void DisableOutlining();
	static void DrawSkybox(Skybox &skybox, const glm::mat4& view, const glm::mat4& projection, Shader& shader);
};