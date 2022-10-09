#include "Renderer.h"
#include "glad/glad.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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