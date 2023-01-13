#include "IndexBuffer.h"
#include <glad/glad.h>
#include "string.h"

IndexBuffer::IndexBuffer() :
	m_count{0}
{
	m_ID = 0;
	m_size = 0;
	m_data = nullptr;
	m_usage = BufferObject::Usage::STATIC;

	glGenBuffers(1, &m_ID);
}

IndexBuffer::IndexBuffer(uint32_t* data, uint32_t count, BufferObject::Usage usage)
	: m_count{ count }
{
	m_ID = 0;
	m_size = m_count * sizeof(uint32_t);
	m_data = data;
	m_usage = usage;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

IndexBuffer::IndexBuffer(const IndexBuffer& ib) :
	m_count{ ib.m_count }
{
	m_ID = 0;
	m_size = ib.m_size;
	m_usage = ib.m_usage;

	m_data = ib.m_data;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

IndexBuffer::IndexBuffer(IndexBuffer&& ib) :
	m_count{ ib.m_count }
{
	m_ID = ib.m_ID;
	m_size = ib.m_size;
	m_data = ib.m_data;
	m_usage = ib.m_usage;

	ib.m_data = nullptr;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& rhs)
{
	if (this == &rhs)
	{
		return *this;
	}

	m_ID = 0;
	m_size = rhs.m_size;
	m_usage = rhs.m_usage;
	m_count = rhs.m_count;

	m_data = rhs.m_data;

	glGenBuffers(1, &m_ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

	switch (m_usage)
	{
	case BufferObject::Usage::DYNAMIC:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_DYNAMIC_DRAW);
		break;

	case BufferObject::Usage::STREAM:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STREAM_DRAW);
		break;

	case BufferObject::Usage::STATIC:
		[[fallthrough]];

	default:
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, m_data, GL_STATIC_DRAW);
		break;
	}
	return *this;
}

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

void IndexBuffer::BindBufferData(void* data, size_t size)
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