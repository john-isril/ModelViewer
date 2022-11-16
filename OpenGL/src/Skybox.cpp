#include "Skybox.h"

Skybox::Skybox(const std::string file_paths[Cubemap::NUM_OF_FACES]) :
	m_VBO{(void*)(m_skybox_vertices), sizeof(m_skybox_vertices)}, m_cubemap{file_paths}
{
	m_Layout.AddAttribute<float>(3);
	m_VAO.AddBufferLayout(m_VBO, m_Layout);
}

Skybox::Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back) :
	m_VBO{ (void*)(m_skybox_vertices), sizeof(m_skybox_vertices) }, m_cubemap{ right, left, top, bottom, front, back }
{
	m_Layout.AddAttribute<float>(3);
	m_VAO.AddBufferLayout(m_VBO, m_Layout);
}

Skybox::~Skybox()
{
}

void Skybox::Activate()
{
	m_VAO.Bind();
	glActiveTexture(GL_TEXTURE0);
	m_cubemap.Bind();
}

void Skybox::Deactivate()
{
	m_VAO.Unbind();
	m_cubemap.Unbind();
}
