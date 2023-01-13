#pragma once
#include <cstdint>

class BufferObject
{
public:
	enum class Usage : uint8_t
	{
		STREAM,
		STATIC,
		DYNAMIC
	};

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void BindBufferData(void* data, size_t size) = 0;

protected:
	uint32_t m_ID;
	size_t m_size;
	void* m_data;
	BufferObject::Usage m_usage;
};

/*
* #include "VertexBuffer.h"
#include <glad/glad.h>
#include <string.h>

VertexBuffer::VertexBuffer() :
	m_ID{}, m_data{nullptr}, m_data_size{0}
{
	glGenBuffers(1, &m_ID);
}

VertexBuffer::VertexBuffer(void* data, size_t size)
	: m_ID{}, m_data{data}, m_data_size{size}
{
	m_data = data;
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, m_data_size, m_data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(const VertexBuffer& vb) :
	m_ID{}, m_data_size{vb.m_data_size}
{
	memcpy(m_data, vb.m_data, m_data_size);
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, m_data_size, m_data, GL_STATIC_DRAW);
}

VertexBuffer::VertexBuffer(VertexBuffer&& vb) :
	m_ID{vb.m_ID}, m_data{vb.m_data}, m_data_size{vb.m_data_size}
{
	vb.m_ID = 0;
	vb.m_data = nullptr;
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_ID = 0;
	m_data_size = rhs.m_data_size;
	memcpy(m_data, rhs.m_data, m_data_size);
	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, m_data_size, m_data, GL_STATIC_DRAW);

	return *this;
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

void VertexBuffer::BindBufferData(void* data, size_t size)
{
	m_data = data;
	m_data_size = size;
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

#pragma once
#include "BufferObject.h"
#include <cstdint>

class VertexBuffer : public BufferObject
{
public:
	VertexBuffer();
	VertexBuffer(void* data, size_t size, BufferObject::Usage usage = BufferObject::Usage::STATIC);
	VertexBuffer(const VertexBuffer& vb);
	VertexBuffer(VertexBuffer&& vb);
	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& rhs);
	void Bind() const;
	void Unbind() const;
	void BindBufferData(void* data, size_t size);
};
*/
