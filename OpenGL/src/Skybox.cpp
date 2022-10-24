#include "Skybox.h"

Skybox::Skybox(const char* cubemap_file_paths[]) :
	m_VBO{(void*)(m_skybox_vertices), sizeof(m_skybox_vertices)}, m_texture{cubemap_file_paths}
{
	m_Layout.AddAttribute<float>(3);
	m_VAO.AddBufferLayout(m_VBO, m_Layout);
}

void Skybox::Activate()
{
	m_VAO.Bind();
	glActiveTexture(GL_TEXTURE0);
	m_texture.Bind();
}

void Skybox::Deactivate()
{
	m_VAO.Unbind();
	m_texture.Unbind();
}
