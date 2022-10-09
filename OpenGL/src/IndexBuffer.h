#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H
#include <cstdint>

class IndexBuffer
{
public:

private:
	uint32_t m_ID;
	uint32_t m_count;

public:
	IndexBuffer(const uint32_t* data, uint32_t count);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	inline uint32_t GetCount() const { return m_count; };

};

#endif