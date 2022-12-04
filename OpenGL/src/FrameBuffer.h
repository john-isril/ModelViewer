#pragma once
#include <cstdint>

class FrameBuffer
{
private:
	uint32_t m_ID;
	uint32_t m_texture_color_buffer_id;
	uint32_t m_render_buffer_id;
	uint32_t m_height;
	uint32_t m_width;
	uint8_t m_subsamples;

public:
	FrameBuffer(uint32_t width, uint32_t height, uint8_t subsamples);
	~FrameBuffer();

	void Bind() const;
	void BindTextureColorBuffer() const;
	void Unbind() const;
	void Resize(uint32_t width, uint32_t height);
	static void ResolveMultsampledFrameBuffer(FrameBuffer* source, FrameBuffer* target);

	uint32_t GetTextureColorBufferID() const;
	uint32_t GetID() const;
	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
};