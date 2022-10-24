#include "IndexBuffer.h"
#include <glad/glad.h>
#include "string.h"

IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count)
	: m_ID{}, m_data{ data }, m_count{ count }
{
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer() :
	m_ID{}, m_data{nullptr}, m_count{ 0 }
{
	glGenBuffers(1, &m_ID);
}

IndexBuffer::IndexBuffer(const IndexBuffer& ib) :
	m_ID {}, m_count{ ib.m_count }
{
	memcpy(m_data, ib.m_data, m_count * sizeof(uint32_t));
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(uint32_t), m_data, GL_STATIC_DRAW);
}

IndexBuffer::IndexBuffer(IndexBuffer&& ib) :
	m_ID { ib.m_ID}, m_data{ ib.m_data }, m_count{ ib.m_count }
{
	ib.m_ID = 0;
	ib.m_data = nullptr;
}
/*
IndexBuffer& IndexBuffer::operator=(const IndexBuffer& ib)
{
	IndexBuffer(ib);
	return *this;
}
*/
IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_ID);
}

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::BindBufferData(uint32_t* data, uint32_t count)
{
	m_count = count;
	m_data = data;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_count * sizeof(uint32_t), data, GL_STATIC_DRAW);
}
