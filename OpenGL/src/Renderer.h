#pragma once
#include <cstdint>

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

	static void Clear();
	static void Clear(float r, float g, float b, float a);
	static void DrawArrays(const class VertexArray& va, const class Shader& shader, uint32_t num_of_vertices);
	static void DrawElements(const class VertexArray& va, const class IndexBuffer& ib, const class Shader& shader);
	static void DrawMesh(const class Mesh& mesh, Shader& shader);
	static void DrawModel(const class Model& model, Shader& shader);
};