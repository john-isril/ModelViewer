#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include <cstdint>

class VertexArray
{
public:
	VertexArray();
//	~VertexArray();
	void AddBufferLayout(const class VertexBuffer& vb, const class VertexBufferLayout& layout);
	void AddBufferLayoutMeshVertex(const class VertexBuffer& vb);
	void Bind() const;
	void Unbind() const;

private:
	uint32_t m_ID;
	uint32_t stride;
};
#endif // !VERTEX_ARRAY_H
