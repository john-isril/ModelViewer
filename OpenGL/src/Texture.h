#pragma once

#include "stb_image.h"
#include <cstdint>
#include <string>
class Texture
{
public:
	enum class Type : uint8_t
	{
		Diffuse,
		Specular,
		Normal,
		Height,
		Roughness,
		Alpha,

		None
	};

	static constexpr uint8_t NUM_OF_CUBE_FACES{ 6 };

public:
	Texture(const std::string &file_path, const std::string &file_name, Type type, bool flip_vertically_on_load);
	Texture(const Texture& texture);

	~Texture();

	void Bind() const;
	void Unbind() const;
	void Load(const char* file_path, bool flip_vertically_on_load);

	uint32_t GetID() const;
	Texture::Type GetType()  const;
	std::string GetFileName() const;

private:
	std::uint32_t m_ID;
	uint8_t *m_buffer;
	int32_t m_width, m_height, m_color_channels;
	Type m_type;
	std::string m_file_name;
	std::string m_file_path;
	bool m_flip_uv;
};