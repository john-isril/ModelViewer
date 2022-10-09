#pragma once
/*#include <glm/glm.hpp>
#include <vector>
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "VertexBufferLayout.h"
#include "Vertex.h"
#include "Texture.h"

class Mesh
{
private:
	std::vector<Vertex> m_vertices;
	std::vector<uint32_t> m_indices;
	std::vector<Texture> m_textures;
	VertexArray m_VAO;
	VertexBuffer m_VBO;
	IndexBuffer m_IBO;
	VertexBufferLayout m_buffer_layout{};

private:
	void Setup();

public:
	Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

public:
};
*/
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
    // mesh Data
    

    // constructor
    Mesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices, std::vector<Texture> textures);

    void Draw(Shader& shader); // todo: this is the renderers responsibility
};