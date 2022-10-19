#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <cstdint>

class VertexBuffer
{
public:
	VertexBuffer();
	VertexBuffer(const void* data, uint32_t size);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
	void BindBufferData(const void* data, uint32_t size);

private:
	unsigned int m_ID;

};

#endif