#include "VertexArray.h"
#include <glad/glad.h>
#include "VertexBufferLayout.h"
#include "VertexBuffer.h"
#include "Mesh.h"

VertexArray::VertexArray()
	: m_ID {}, stride {}
{
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
}

VertexArray::VertexArray(const VertexArray& va)
	: m_ID{}, stride{}
{
	glGenVertexArrays(1, &m_ID);
	glBindVertexArray(m_ID);
}

void VertexArray::AddBufferLayout(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	vb.Bind();
	const auto& attributes{ layout.GetElements() };

	std::uint32_t offset{};

	for (size_t i = 0; i < attributes.size(); i++)
	{
		glVertexAttribPointer(i, attributes[i].count, attributes[i].type, attributes[i].normalized, layout.GetStride(), (const void*)(offset));
		offset += attributes[i].count * Attribute::GetGLSize(attributes[i].type);
		glEnableVertexAttribArray(i);
	}
}

void VertexArray::AddBufferLayoutMeshVertex(const VertexBuffer& vb)
{
	vb.Bind();
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinates));

	// vertex tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	
	// vertex bitangent
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));
	
	// bone ids
	//glEnableVertexAttribArray(5);
	//glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs));

	// weights
	//glEnableVertexAttribArray(6);
	//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights));
	//glBindVertexArray(0);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_ID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_ID);
}
//FIX: destroys when out of scope since were returning copy 