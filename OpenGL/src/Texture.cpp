#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const std::string& file_path, const std::string& file_name, Type type, bool flip_vertically_on_load)
	: m_ID{}, m_buffer{ nullptr }, m_width{}, m_height{}, m_color_channels{}, m_type{ type }, m_file_name{ file_name }, m_file_path{file_path}, m_flip_uv{flip_vertically_on_load}
{
	glGenTextures(1, &m_ID);
	this->Load(file_path.c_str(), flip_vertically_on_load);
}

Texture::Texture(const Texture& texture) :
	Texture(texture.m_file_path, texture.m_file_name, texture.m_type, texture.m_flip_uv)
{
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, m_ID);
}

void Texture::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Load(const char* file_path, bool flip_vertically_on_load)
{
	stbi_set_flip_vertically_on_load(flip_vertically_on_load);

	m_buffer = stbi_load(file_path, &m_width, &m_height, &m_color_channels, 0);
	if (m_buffer)
	{
		this->Bind();

		switch (m_color_channels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_width, m_height, 0, GL_RED, GL_UNSIGNED_BYTE, m_buffer);
			break;

		case 3:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
			break;

		case 4:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_buffer);
			break;

		default:
			std::cerr << "Error: Texture format unspecified.\n";
			stbi_image_free(m_buffer);
			return;

		}

		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else
	{
		std::cerr << "Failed to load texture\n";
	}

	stbi_image_free(m_buffer);
}

uint32_t Texture::GetID() const
{
	return m_ID;
}

Texture::Type Texture::GetType() const
{
	return m_type;
}

std::string Texture::GetFileName() const
{
	return m_file_name;
}
