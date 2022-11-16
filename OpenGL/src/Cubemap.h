#pragma once
#include "Texture.h"
#include <string>

class Cubemap
{
public:
	static constexpr uint8_t NUM_OF_FACES{ 6 };

	Cubemap(const std::string file_paths[NUM_OF_FACES]);
	Cubemap(const std::string &right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);
	~Cubemap();

	void Bind() const;
	void Unbind() const;
	void Load(const std::string file_paths[NUM_OF_FACES]);
	void Load(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom, const std::string& front, const std::string& back);
	void LoadFront(const std::string& path);
	void LoadLeft(const std::string& path);
	void LoadRight(const std::string& path);
	void LoadTop(const std::string& path);
	void LoadBottom(const std::string& path);
	void LoadBack(const std::string& path);

	uint32_t GetID() const;
	const std::string* GetFileNames() const;

private:
	std::string m_file_paths[NUM_OF_FACES];
	std::uint32_t m_ID;
	uint8_t* m_buffer;
	int32_t m_width, m_height, m_color_channels;
	std::string m_file_name;
	std::string m_file_path;
};