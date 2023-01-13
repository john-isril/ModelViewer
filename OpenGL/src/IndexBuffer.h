#pragma once
#include "BufferObject.h"
#include <cstdint>

class IndexBuffer : public BufferObject
{
public:
	IndexBuffer();
	IndexBuffer(uint32_t* data, uint32_t count, BufferObject::Usage usage = BufferObject::Usage::STATIC);
	IndexBuffer(const IndexBuffer& ib);
	IndexBuffer(IndexBuffer&& ib);
	~IndexBuffer();

	IndexBuffer& operator=(const IndexBuffer& rhs);
	void Bind() const;
	void Unbind() const;
	void BindBufferData(void* data, size_t size);
	inline uint32_t GetCount() const { return m_count; };

private:
	uint32_t m_count;
};