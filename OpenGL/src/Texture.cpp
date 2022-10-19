#include "Texture.h"
#include <glad/glad.h>
#include <iostream>

Texture::Texture(const char *file_path, bool flip_vertically_on_load, Type type, std::string file_name)
	: m_ID{}, m_buffer{ nullptr }, m_width{}, m_height{}, m_color_channels{}, m_type{ type }, m_file_name{ file_name }
{
	this->Load(file_path, flip_vertically_on_load);
}

Texture::Texture(const Texture& texture) :
	m_ID{ texture.m_ID }, m_buffer{ texture.m_buffer }, m_width{ texture.m_width }, m_height{ texture.m_height }, m_color_channels{}, m_type{ texture.m_type }, m_file_name{ texture.m_file_name }
{
}

Texture::Texture(const char* file_paths[]) :
	m_ID{}, m_buffer{ nullptr }, m_width{}, m_height{}, m_color_channels{}, m_type{ Texture::Type::CubeMap }
{
	this->LoadCubeMap(file_paths);
}

Texture::~Texture()
{
	//glDeleteTextures(1, &m_ID);
}

void Texture::Bind() const
{
	if (m_type == Texture::Type::CubeMap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, m_ID);
	}
}

void Texture::Unbind() const
{
	if (m_type == Texture::Type::CubeMap)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void Texture::Load(const char* file_path, bool flip_vertically_on_load)
{
	glGenTextures(1, &m_ID);

	stbi_set_flip_vertically_on_load(true);

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

void Texture::LoadCubeMap(const char* file_paths[])
{
	glGenTextures(1, &m_ID);
	this->Bind();

	stbi_set_flip_vertically_on_load(false);
	
	for (uint8_t i{ 0 }; i < NUM_OF_CUBE_FACES; ++i)
	{
		m_buffer = stbi_load(file_paths[i], &m_width, &m_height, &m_color_channels, 0);
		
		if (m_buffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
		}
		else
		{
			std::cerr << "Failed to load cubemap texture at file \n" << file_paths[i];
		}

		stbi_image_free(m_buffer);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
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
