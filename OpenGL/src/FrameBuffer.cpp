#include "FrameBuffer.h"
#include <glad/glad.h>
#include <iostream>

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, uint8_t subsamples) :
    m_ID{}, m_render_buffer_id{}, m_texture_color_buffer_id{}, m_width{ width }, m_height{ height }, m_subsamples{ subsamples }
{
	glGenFramebuffers(1, &m_ID) ;
	glBindFramebuffer(GL_FRAMEBUFFER, m_ID);

    glGenTextures(1, &m_texture_color_buffer_id);

    if (m_subsamples)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture_color_buffer_id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_subsamples, GL_RGB, m_width, m_height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_texture_color_buffer_id, 0);

        glGenRenderbuffers(1, &m_render_buffer_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_subsamples, GL_DEPTH24_STENCIL8, m_width, m_height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture_color_buffer_id, 0);

        glGenRenderbuffers(1, &m_render_buffer_id);
        glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_render_buffer_id);
    }
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "ERROR::FRAMEBUFFER:: Incomplete Frame Buffer\n";
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	glDeleteFramebuffers(1, &m_ID);
}

void FrameBuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_ID);
    glEnable(GL_DEPTH_TEST);
}

void FrameBuffer::BindTextureColorBuffer() const
{
    if (m_subsamples)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture_color_buffer_id);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer_id);
    }
}

void FrameBuffer::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void FrameBuffer::Resize(uint32_t width, uint32_t height)
{
    m_width = width;
    m_height = height;

    if (m_subsamples)
    {
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_texture_color_buffer_id);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_subsamples, GL_RGB, m_width, m_height, GL_TRUE);

        glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_subsamples, GL_DEPTH24_STENCIL8, m_width, m_height);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, m_texture_color_buffer_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindRenderbuffer(GL_RENDERBUFFER, m_render_buffer_id);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height);
    }
}

void FrameBuffer::ResolveMultsampledFrameBuffer(FrameBuffer* source, FrameBuffer* target)
{
    if (source && target)
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, source->GetID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target->GetID());
        glBlitFramebuffer(0, 0, target->GetWidth(), target->GetHeight(), 0, 0, target->GetWidth(), target->GetHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }
}

uint32_t FrameBuffer::GetTextureColorBufferID() const
{
    return m_texture_color_buffer_id;
}

uint32_t FrameBuffer::GetID() const
{
    return m_ID;
}

uint32_t FrameBuffer::GetWidth() const
{
    return m_width;
}

uint32_t FrameBuffer::GetHeight() const
{
    return m_height;
}
