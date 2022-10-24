#pragma once

#include <cstdint>

class IndexBuffer
{
public:
	IndexBuffer(uint32_t* data, uint32_t count);
	IndexBuffer();
	IndexBuffer(const IndexBuffer& ib);
	IndexBuffer(IndexBuffer&& ib);
	~IndexBuffer();

	//IndexBuffer& operator=(const IndexBuffer& ib);
	void Bind() const;
	void Unbind() const;
	void BindBufferData(uint32_t* data, uint32_t count);
	inline uint32_t GetCount() const { return m_count; };

private:
	uint32_t m_ID;
	uint32_t m_count;
	uint32_t* m_data;

};