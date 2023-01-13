#include "VertexBuffer.h"
#include <glad/glad.h>
#include <string.h>

VertexBuffer::VertexBuffer()
{
	m_ID = 0;
	m_size = 0;
	m_data = nullptr;
	m_usage = BufferObject::Usage::STATIC;

	glGenBuffers(1, &m_ID);
}

VertexBuffer::VertexBuffer(void* data, size_t size, BufferObject::Usage usage)
{
	m_ID = 0;
	m_size = size;
	m_data = data;
	m_usage = usage;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);
	// Usage is a hint to tell the GPU how the buffer data will be used.
	// GL_STATIC_DRAW: Data in the buffer object will be set once and used many times.
	// GL_DYNAMIC_DRAw: Data in the buffer object will be set many times and used many times.
	// GL_stream_DRAw: Data in the buffer object will be set once and used once times.

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

VertexBuffer::VertexBuffer(const VertexBuffer& vb)
{
	m_ID = 0;
	m_size = vb.m_size;
	m_usage = vb.m_usage;

	m_data = vb.m_data;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

VertexBuffer::VertexBuffer(VertexBuffer&& vb)
{
	m_ID = vb.m_ID;
	m_size = vb.m_size;
	m_data = vb.m_data;
	m_usage = vb.m_usage;

	vb.m_data = nullptr;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_ID = 0;
	m_size = rhs.m_size;
	m_usage = rhs.m_usage;

	m_data = rhs.m_data;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}

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
	m_size = size;
	glBindBuffer(GL_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}