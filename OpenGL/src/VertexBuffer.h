#pragma once

#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(void* data, size_t size);
	VertexBuffer(const VertexBuffer& vb);
	VertexBuffer(VertexBuffer&& vb);
	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& rhs);
	void Bind() const;
	void Unbind() const;
	void BindBufferData(void* data, size_t size);

private:
	uint32_t m_ID;
	size_t m_data_size;
	void* m_data;
};