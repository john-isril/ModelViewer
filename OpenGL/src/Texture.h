#ifndef TEXTURE_H
#define TEXTURE_H

#include "stb_image.h"
#include <cstdint>
#include <string>
class Texture
{
public:
	enum class Format : uint8_t
	{
		JPG, PNG, None
	};

	enum class Type : uint8_t
	{
		Diffuse,
		Specular,
		Normal,
		Height,
		Roughness,
		CubeMap
	};

	static constexpr uint8_t NUM_OF_CUBE_FACES{ 6 };

public:
	Texture(const char* file_path, bool flip_vertically_on_load, Type type, std::string file_name);
	Texture(const Texture& texture);
	Texture(const char *file_paths[]);

	~Texture();

	void Bind() const;
	void Unbind() const;
	void Load(const char* file_path, bool flip_vertically_on_load);
	void LoadCubeMap(const char* file_paths[]);

	uint32_t GetID() const;
	Texture::Type GetType()  const;
	std::string GetFileName() const;

private:
	std::uint32_t m_ID;
	uint8_t *m_buffer;
	int32_t m_width, m_height, m_color_channels;
	Type m_type;
	std::string m_file_name;
};
#endif // !TEXTURE_H
