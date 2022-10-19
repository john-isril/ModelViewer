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
	IndexBuffer();
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
	void BindBufferData(const uint32_t* data, uint32_t count);
	inline uint32_t GetCount() const { return m_count; };

};

#endif