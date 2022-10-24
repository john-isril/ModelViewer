#ifndef VERTEX_BUFFER_LAYOUT_H
#define VERTEX_BUFFER_LAYOUT_H
#include <cstdint>
#include <iostream>
#include <vector>
#include <glad/glad.h>

typedef struct Attribute
{
	uint32_t count;
	uint32_t type;
	uint8_t normalized;

	static size_t GetGLSize(unsigned int type)
	{
		switch (type)
		{
		case (GL_FLOAT): return sizeof(GL_FLOAT);
		case (GL_INT): return sizeof(GL_INT);
		case (GL_UNSIGNED_INT): return sizeof(GL_UNSIGNED_INT);
		}
	}
} Attribute;

class VertexBufferLayout
{
private:
	unsigned int m_stride;
	std::vector<Attribute> m_attributes;
public:
	VertexBufferLayout()
		: m_stride {} {}

	~VertexBufferLayout()
	{}

	template<typename T>
	void AddAttribute(uint32_t count)
	{}

	template<>
	void AddAttribute<float>(uint32_t count)
	{
		m_attributes.push_back({ count, GL_FLOAT, false });
		m_stride += (count * sizeof(GL_FLOAT));
	}

	template<>
	void AddAttribute<int>(uint32_t count)
	{
		m_attributes.push_back({ count, GL_INT, false });
		m_stride += (count * sizeof(GL_INT));
	}

	template<>
	void AddAttribute<uint32_t>(uint32_t count)
	{
		m_attributes.push_back({ count, GL_UNSIGNED_INT, false });
		m_stride += (count * sizeof(GL_UNSIGNED_INT));
	}

	inline const std::vector<Attribute>& GetElements() const
	{
		return m_attributes;
	}

	inline uint32_t GetStride() const
	{
		return m_stride;
	}
};

#endif // !VERTEX_BUFFER_LAYOUT_H
