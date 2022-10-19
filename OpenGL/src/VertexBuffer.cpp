#include "VertexBuffer.h"
#include <glad/glad.h>

VertexBuffer::VertexBuffer() :
	m_ID{}
{
	glGenBuffers(1, &m_ID);
}

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
	: m_ID{}
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}



VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
}

void VertexBuffer::Unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::BindBufferData(const void* data, uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}
