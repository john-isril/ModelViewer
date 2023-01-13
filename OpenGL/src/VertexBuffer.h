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
	virtual void Bind() const override;
	virtual void Unbind() const override;
	virtual void BindBufferData(void* data, size_t size) override;
};