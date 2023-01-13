#include "Cubemap.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

Cubemap::Cubemap(const std::string file_paths[NUM_OF_FACES]) :
	m_ID{}, m_buffer{ nullptr }, m_width{}, m_height{}, m_color_channels{}
{
	glGenTextures(1, &m_ID);
	this->Load(file_paths);
}

Cubemap::Cubemap(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back) :
	m_ID{}, m_buffer{ nullptr }, m_width{}, m_height{}, m_color_channels{}
{
	glGenTextures(1, &m_ID);
	this->Load(right, left, top, bottom, front, back);
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &m_ID);
}

void Cubemap::Bind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
}

void Cubemap::Unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Cubemap::Load(const std::string file_paths[NUM_OF_FACES])
{
	this->Bind();

	stbi_set_flip_vertically_on_load(false);

	for (uint8_t i{ 0 }; i < NUM_OF_FACES; ++i)
	{
		m_buffer = stbi_load(file_paths[i].c_str(), &m_width, &m_height, &m_color_channels, 0);

		if (m_buffer)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
			m_file_paths[i] = file_paths[i];
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

void Cubemap::Load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
{
	this->Bind();

	stbi_set_flip_vertically_on_load(false);
	LoadRight(right);
	LoadLeft(left);
	LoadTop(top);
	LoadBottom(bottom);
	LoadFront(front);
	LoadBack(back);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void Cubemap::LoadFront(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

void Cubemap::LoadBack(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

void Cubemap::LoadLeft(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

void Cubemap::LoadRight(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

void Cubemap::LoadTop(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

void Cubemap::LoadBottom(const std::string& path)
{
	m_buffer = stbi_load(path.c_str(), &m_width, &m_height, &m_color_channels, 0);

	if (m_buffer)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_SRGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_buffer);
	}
	else
	{
		std::cerr << "Failed to load cubemap texture at file \n" << path;
	}

	stbi_image_free(m_buffer);
}

uint32_t Cubemap::GetID() const
{
	return m_ID;
}

const std::string* Cubemap::GetFileNames() const
{
	return m_file_paths;
}
